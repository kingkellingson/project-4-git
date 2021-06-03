#ifndef LEXER_H
#define LEXER_H
#include <vector>
#include <iostream>
#include "Automaton.h"
#include "Token.h"
#include "MatcherAutomaton.h"
#include "StringAutomaton.h"
#include "IDAutomaton.h"
#include "LineCommentAutomaton.h"
#include "BlockCommentAutomaton.h"

using namespace std;

class Lexer
{
private:
    std::vector<Automaton*> nonMatchingAutomata;
    std::vector<Automaton*> matchingAutomata;
    std::vector<Token*> tokens;
    int lineNumber = 0;
    int maxRead = 0;
    int inputRead = 0;
    Automaton* maxAutomaton = nullptr;
    string mainInput;
    string tokenDescription;
    Token* new_Token;


    void CreateAutomata();

    // TODO: add any other private methods here (if needed)

public:
    Lexer(string input) : mainInput (input) {
        CreateAutomata();
        Run(mainInput);//Runs the Lexer on the input
        //prints out all of the tokens FIXME: Disabling the printing for pass off of Lab 2
        /*for (size_t i = 0; i < tokens.size(); ++i)
        {
            if (i==0)
            {
                tokens.at(i)->toString();
            }
            else {
                cout << endl;
                tokens.at(i)->toString();
            }
        }
        cout << endl << "Total Tokens = " << tokens.size();*/
    }
    ~Lexer()
    {
        // TODO: need to clean up the memory in `automata` and `tokens`
    }

    std::vector<Token*> getVectorOfTokens();
    void Run(std::string& input);
    void lookForMatch(std::string& input);
    Token* CreateUndefinedToken(std::string input, int lineNumber) { return new Token(TokenType::UNDEFINED, input, lineNumber); }
    Token* CreateEOFToken(std::string input, int lineNumber) { return new Token(TokenType::EOF_TYPE, input, lineNumber); }

    // TODO: add other public methods here

};

std::vector<Token*> Lexer::getVectorOfTokens()
{
    return tokens;
}

void Lexer::Run(std::string& input) {
    lineNumber = 1;
    // While there are more characters to tokenize AKA We have not reached the end of the file
    while (input != "EOF_TYPE") {
    //loop while input.size() > 0 {
        maxRead = 0;
        //maxAutomaton = nonMatchingAutomata.front(); ///FIXME: is this okay?
        maxAutomaton = nullptr;

        ///This handles whitespace in between tokens//FIXME: could be a problem with strings
        if (input.at(0)== ' ')
        {
            input = input.substr(1);
            continue;
        }
        if (input.at(0)== '\t')
        {
            input = input.substr(1);
            continue;
        }

        /// Here is the "Parallel" part of the algorithm
        ///   Try on every matching string for a matching automaton
        lookForMatch (input);

        ///   Each remaining automaton runs with the same input
        for (size_t i = 0; i < nonMatchingAutomata.size(); ++i) {
            inputRead = nonMatchingAutomata.at(i)->Start(input); //Start the automaton at the first position.
            if (inputRead > maxRead) { //if it has read the greatest amount
                maxRead = inputRead; //it becomes the winning automaton
                maxAutomaton = nonMatchingAutomata.at(i);
            }
        }

        /// Here is the "Max" part of the algorithm
        if (maxRead > 0) {
            tokenDescription = input.substr(0, maxRead); //reads the description of the token
            new_Token = maxAutomaton->CreateToken(tokenDescription, lineNumber); ///Create
            //TODO increment lineNumber by maxAutomaton.NewLinesRead():: DONE
            lineNumber = lineNumber + maxAutomaton->NewLinesRead();
            //FIXME: This is all added in order to make project 2 work
            if (new_Token->getMyTypeToPrint() != "COMMENT")
            {
                tokens.push_back(new_Token); //add newToken to collection of all tokens
            }
        }
        ///run into an endline;
        else if (input.substr(0,6) == "0endl0")
        {
            input = input.substr(6);
            lineNumber++;
        }
        /// No automaton accepted input
        /// Create single character undefined token
        else {
            maxRead = 1;
            tokenDescription = input.substr(0, maxRead);
            new_Token = CreateUndefinedToken(tokenDescription, lineNumber);
                //add newToken to collection of all tokens
            tokens.push_back(new_Token);
        }

        /// Update `input` by removing characters read to create Token
        input = input.substr(maxRead);
    }
    new_Token = CreateEOFToken("", lineNumber);
    tokens.push_back(new_Token);
}


////////////////////////////////////////////////
///This just implements Matcher Automaton///
////////////////////////////////////////////////
void Lexer::lookForMatch (std::string& input)
{
    string myMatch;
    TokenType myType;
    for (size_t i = 0; i < matchingAutomata.size(); ++i) {
        switch (i) {
            case 0://Comma
                myMatch = ",";
                myType = TokenType::COMMA;
                break;
            case 1://Period
                myMatch = ".";
                myType = TokenType::PERIOD;
                break;
            case 2://Q_Mark
                myMatch = "?";
                myType = TokenType::Q_MARK;
                break;
            case 3://Left_Paren
                myMatch = "(";
                myType = TokenType::LEFT_PAREN;
                break;
            case 4://Right_Paren
                myMatch = ")";
                myType = TokenType::RIGHT_PAREN;
                break;
            case 5://Colon
                myMatch = ":";
                myType = TokenType::COLON;
                break;
            case 6://Colon_Dash
                myMatch = ":-";
                myType = TokenType::COLON_DASH;
                break;
            case 7://Multiply
                myMatch = "*";
                myType = TokenType::MULTIPLY;
                break;
            case 8://Add
                myMatch = "+";
                myType = TokenType::ADD;
                break;
            case 9://Schemes
                myMatch = "Schemes";
                myType = TokenType::SCHEMES;
                break;
            case 10://Facts
                myMatch = "Facts";
                myType = TokenType::FACTS;
                break;
            case 11://Rules
                myMatch = "Rules";
                myType = TokenType::RULES;
                break;
            case 12://Queries
                myMatch = "Queries";
                myType = TokenType::QUERIES;
                break;
            default:
                myMatch = "Error: Outside of Vector!";
        }
        matchingAutomata.at(i)->setStringToMatch(myMatch); //Set the string to match
        matchingAutomata.at(i)->setTokenType(myType); //Set type of the token
        inputRead = matchingAutomata.at(i)->Start(input); //Start the automaton at the first position.
        if (inputRead > maxRead) { //if it has read the greatest amount
            maxRead = inputRead; //it becomes the winning automaton
            maxAutomaton = matchingAutomata.at(i);
        }
    }
}

///This populates a vector with the Automata I've defined
void Lexer::CreateAutomata() {
    for (int i = 0; i < NUMBER_OF_STRING_TOKENS; ++i)
    {
        matchingAutomata.push_back(new MatcherAutomaton());
    }
    nonMatchingAutomata.push_back(new StringAutomaton());
    nonMatchingAutomata.push_back(new IDAutomaton());
    nonMatchingAutomata.push_back(new LineCommentAutomaton());
    nonMatchingAutomata.push_back(new BlockCommentAutomaton());
    // TODO: Add the other needed automata here
}

#endif // LEXER_H

