//
// Created by Kyle Ellingson on 5/5/21.
//

#ifndef PROJECT_0_IDAUTOMATON_H
#define PROJECT_0_IDAUTOMATON_H

#include "Automaton.h"

class IDAutomaton : public Automaton
{
private:
    void S1(std::string& input);

public:
    IDAutomaton() : Automaton(TokenType::ID) {}  // Call the base constructor

    void S0(std::string& input);
};

void IDAutomaton::S0(std::string& input) {
    if (isalpha(input[index])) {
        inputRead++;
        index++;
        S1(input);
    }
    else {
        Serr();
    }
}

void IDAutomaton::S1(std::string& input) { //Loops the middle of the string. Error if not letter or digit

    for (size_t i = 5; i<8; ++i)
    {
        switch (i)
        {
            case 5:
                if (input.substr(index, i)=="Facts"||input.substr(index, i)=="Rules")
                {
                    //if it either has a space next or an endline character next, then it ran into an existing token.
                    if (!isalnum(input.at(index+1))||input.substr(index, 6)=="0endl0") { return; }
                    //if not, then it must be a part of the ID
                    else { break; }
                }
            case 6:
                if (input.substr(index, i)=="0endl0")
                {
                    return; //return because it has reached the end of the ID token
                }
            case 7:
                if (input.substr(index, i)=="Schemes"||input.substr(index, i)=="Queries")
                {
                    //if it either has a space next or an endline character next, then it ran into an existing token.
                    if (!isalnum(input.at(index+1))||input.substr(index, 6)=="0endl0") { return; }
                    //if not, then it must be a part of the ID
                    else { break; }
                }
            /*case 8:
                if (input.substr(index, i)=="EOF_TYPE")
                {
                    if (!isalnum(input.at(index+1))||input.substr(index, 6)=="0endl0") { return; }
                    else { break; }
                }*////don't think it will ever run into my EOF_TYPE token
            default: ;
        }
    }

    if (isalnum(input[index])) {//If it runs into an alphanumeric symbol
        inputRead++;
        index++;
        S1(input);
    }
    else {
        return;
    }
}


#endif //PROJECT_0_IDAUTOMATON_H
