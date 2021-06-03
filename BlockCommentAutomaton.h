//
// Created by Kyle Ellingson on 5/6/21.
//

#ifndef PROJECT_0_BLOCKCOMMENTAUTOMATON_H
#define PROJECT_0_BLOCKCOMMENTAUTOMATON_H

#include "Automaton.h"

class BlockCommentAutomaton : public Automaton
{
private:
    void S1(std::string& input);
    void S2(std::string& input);
    void S3(std::string& input);

public:
    BlockCommentAutomaton() : Automaton(TokenType::COMMENT) {}  // Call the base constructor

    void S0(std::string& input);
};

void BlockCommentAutomaton::S0(std::string& input) {
    if (input[index] == '#') {
        inputRead++;
        index++;
        S1(input);
    }
    else {
        Serr();
    }
}

void BlockCommentAutomaton::S1(std::string& input) { //Loops the middle of the string. Exit if an apostrophe is found
    if (input[index] == '|') {
        inputRead++;
        index++;
        S2(input);
    }
    else {
        Serr();
    }
}

void BlockCommentAutomaton::S2(std::string& input) { //Loops the middle of the string. Exit if a pipe is found
    if (input.substr(index, 6)=="0endl0")
    {
        input = input.replace (index, 6, "\n");
        newLines++;
        S2(input);//here I need it to keep going on the new input,
        return; //I need it to not continue if it has found an endl.
    }
    else if (input.substr(index, 8)=="EOF_TYPE")
    {
        setTokenType(TokenType::UNDEFINED);
        return;
    }

    if (input[index] != '|') {//stay if not found
        inputRead++;
        index++;
        S2(input);
    }
    else {//exit if found
        inputRead++;
        index++;
        S3(input);
    }
}

void BlockCommentAutomaton::S3(std::string& input) { //Loops the middle of the string. Exit if an apostrophe is found
    if (input.substr(index, 6)=="0endl0")
    {
        input = input.replace (index, 6, "\n");
        newLines++;
        S2(input);//here I need it to keep going on the new input,
        return; //I need it to not continue if it has found an endl.
    }

    if (input[index] != '#') {//return to State 2
        inputRead++;
        index++;
        S2(input);
    }
    else {//done
        inputRead++;
        index++;
        return;
    }
}

#endif //PROJECT_0_BLOCKCOMMENTAUTOMATON_H
