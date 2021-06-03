//
// Created by Kyle Ellingson on 5/6/21.
//

#ifndef PROJECT_0_LINECOMMENTAUTOMATON_H
#define PROJECT_0_LINECOMMENTAUTOMATON_H

#include "Automaton.h"

class LineCommentAutomaton : public Automaton
{
private:
    void S1(std::string& input);
    void S2(std::string& input);

public:
    LineCommentAutomaton() : Automaton(TokenType::COMMENT) {}  // Call the base constructor

    void S0(std::string& input);
};

void LineCommentAutomaton::S0(std::string& input) {
    if (input[index] == '#') {
        inputRead++;
        index++;
        S1(input);
    }
    else {
        Serr();
    }
}

void LineCommentAutomaton::S1(std::string& input) { //Loops the middle of the string. Exit if an apostrophe is found
    if (input[index] != '|') {
        inputRead++;
        index++;
        S2(input);
    }
    else {
        Serr();
    }
}

void LineCommentAutomaton::S2(std::string& input) { //Finishes if and endline character is found,
    if (input.substr(index, 6) != "0endl0") {
        inputRead++;
        index++;
        S2(input);
    }
    else {
        return;
    }
}

#endif //PROJECT_0_LINECOMMENTAUTOMATON_H
