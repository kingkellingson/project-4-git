//
// Created by Kyle Ellingson on 5/4/21.
//

#ifndef PROJECT_0_MATCHERAUTOMATON_H
#define PROJECT_0_MATCHERAUTOMATON_H
using namespace std;
#include <string>

class MatcherAutomaton :
        public Automaton
{
//private:
  //  std::string toMatch;
public:
    MatcherAutomaton() : Automaton(TokenType::MATCHER) {} //Calls the base constructor

    void S0( std::string& input)
    {
        bool isMatch = true;
        inputRead = 0; // member variable inherited from Automaton class
        for (int i = 0; i < (int)toMatch.size() && isMatch; i++) {
            if (input[i] != toMatch[i]) {
                isMatch = false;
            }
        }
        if (isMatch) {
            inputRead = toMatch.size();
        }
    }
};

#endif //PROJECT_0_MATCHERAUTOMATON_H
