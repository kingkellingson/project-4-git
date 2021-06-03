//
// Created by Kyle Ellingson on 5/5/21.
//

#ifndef PROJECT_0_STRINGAUTOMATON_H
#define PROJECT_0_STRINGAUTOMATON_H

#include "Automaton.h"

class StringAutomaton : public Automaton
{
private:
    void S1(std::string& input);
    void S2(std::string& input);

public:
    StringAutomaton() : Automaton(TokenType::STRING) {}  // Call the base constructor

    void S0(std::string& input);
};

void StringAutomaton::S0(std::string& input) {
    if (input[index] == '\'') {
        inputRead++;
        index++;
        S1(input);
    }
    else {
        Serr();
    }
}

void StringAutomaton::S1(std::string& input) { //Loops the middle of the string. Exit if an apostrophe is found
    if (input.substr(index, 6)=="0endl0")
    {
        input = input.replace (index, 6, "\n");
        newLines++;
        S1(input);//here I need it to keep going on the new input,
        return; //I need it to not continue if it has found an endl.
    }
    else if (input.substr(index, 8)=="EOF_TYPE")
    {
        setTokenType(TokenType::UNDEFINED);
        return;
    }

    if (input[index] != '\'') {//stay if not found
        inputRead++;
        index++;
        S1(input);
    }
    else if (input[index] == '\'') {//exit if found
        inputRead++;
        index++;
        S2(input);
    }
    else {
        Serr();
    }
}

void StringAutomaton::S2(std::string& input) { //Finishes if apostrophe not found,
                                                     // still in loop if followed by another apostrophe.
    //if (input[index] != '\'' || input.substr(index, 6) == "0endl0") {
    if (input[index] != '\'') {//done if the next input is not an apostrophe
        //do nothing, because it already counted that string
    }
    else if (input[index] == '\'') {//if it is an apostrophe, it must be a double apostrophe.
        inputRead++;
        //input = input.replace (index, 1, "");
        index++;
        S1(input);//Returns to loop
    }
    else {
        Serr();
    }
}


#endif //PROJECT_0_STRINGAUTOMATON_H
