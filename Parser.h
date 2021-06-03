//
// Created by Kyle Ellingson on 5/12/21.
//

#ifndef PROJECT_0_PARSER_H
#define PROJECT_0_PARSER_H

#include <vector>
#include <set>
#include <iterator>
#include <iostream>
#include <sstream>
#include "Token.h"
#include "DataStructures.h"

using namespace std;


class DatalogProgram
{
private:
    vector<Token*>& tokens;
    std::size_t index = 0;
    vector<Predicate*> mySchemes;
    vector<Predicate*> myFacts;
    vector<Rule*> myRules;
    vector<Predicate*> myQueries;
    set<string> myDomain;
    char flag;
public:
    DatalogProgram (vector<Token*>& lexerTokens) : tokens(lexerTokens)
    //SCHEMES COLON scheme schemeList FACTS COLON factList RULES COLON ruleList QUERIES COLON query queryList EOF
    {};

    vector<Predicate*>& getSchemes ()
    {
        return mySchemes;
    }

    vector<Predicate*>& getFacts ()
    {
        return myFacts;
    }

    vector<Predicate*>& getQueries ()
    {
        return myQueries;
    }

    vector<Rule*>& getRules ()
    {
        return myRules;
    }

    void toString ()
    {
        cout << "Success!";
        cout << endl << "Schemes(" << mySchemes.size() << "):";
        for (size_t i = 0; i<mySchemes.size(); ++i)
        {
            cout << endl << "  ";
            mySchemes.at(i)->toString();
        }
        cout << endl << "Facts(" << myFacts.size() << "):";
        for (size_t i = 0; i<myFacts.size(); ++i)
        {
            cout << endl << "  ";
            myFacts.at(i)->toString();
            cout << ".";
        }
        cout << endl << "Rules(" << myRules.size() << "):";
        for (size_t i = 0; i<myRules.size(); ++i)
        {
            myRules.at(i)->toString();
            cout << ".";
        }
        cout << endl << "Queries(" << myQueries.size() << "):";
        for (size_t i = 0; i<myQueries.size(); ++i)
        {
            cout << endl << "  ";
            myQueries.at(i)->toString();
            cout << "?";
        }
        cout << endl << "Domain(" << myDomain.size() << "):";
        for (set<string>::iterator iter = myDomain.begin(); iter != myDomain.end(); ++iter)
        {
            cout << endl << "  " << *iter;
        }
    }

    void begin()
    {//SCHEMES COLON scheme schemeList FACTS COLON factList RULES COLON ruleList QUERIES COLON query queryList EOF
       //SCHEMES
       match (tokens.at(index), "SCHEMES"); //SCHEMES is in the first set of SCHEMES
       //COLON
       match (tokens.at(index), "COLON"); //COLON is in the first set of COLON
       ///scheme -> ID LEFT_PAREN ID idList RIGHT_PAREN
       flag = 'S';
       scheme ();
       ///schemeList -> scheme schemeList | lambda
       schemeList ();
       //FACTS
       match (tokens.at(index), "FACTS"); //FACTS is in the first set of FACTS
       //COLON
       match (tokens.at(index), "COLON"); //COLON is in the first set of COLON
       ///factList -> fact factList | lambda
       flag = 'F';
       factList ();
       //RULES
       match (tokens.at(index), "RULES"); //RULES is in the first set of RULES
       //COLON
       match (tokens.at(index), "COLON"); //COLON is in the first set of COLON
       ///ruleList -> rule ruleList | lambda
       flag = 'R';
       ruleList ();
       //QUERIES
       match (tokens.at(index), "QUERIES"); //QUERIES is in the first set of QUERIES
       //COLON
       match (tokens.at(index), "COLON"); //COLON is in the first set of COLON
       ///query -> predicate Q_MARK
       flag = 'Q';
       query ();
       ///queryList -> query queryList | lambda
       queryList ();
    }

    void match(Token*& currentToken, string toMatch)
    {
        if (currentToken->getMyTypeToPrint() == toMatch)
        {
            ++index;
            return;
        }
        else
        {
            throw tokens.at(index);
        }
    }

    void queryList ()
    {
        //query queryList | lambda
        if (tokens.at(index)->getMyTypeToPrint() == "ID")//ID is in the first set of queryList
        {
            query();
            queryList();
            return;
        }
        else if (tokens.at(index)->getMyTypeToPrint() == "EOF")//EOF is in the follow set of queryList
        {
            return; //just pop, do not advance the input.
        }
        else
        {
            throw tokens.at(index);
        }
    }

    void query ()
    {
        //predicate Q_MARK
        //predicate
        predicate ();
        //Q_MARK
        match (tokens.at(index), "Q_MARK"); //Q_MARK is in the first set of Q_MARK
    }

    void idList ()
    {
        //COMMA ID idList | lambda
        if (tokens.at(index)->getMyTypeToPrint() == "COMMA")//COMMA is in the first set of idlist
        {
        match (tokens.at(index), "COMMA"); //COMMA is in the first set of COMMA
        //ID
        match (tokens.at(index), "ID"); //ID is in the first set of ID
        switch (flag)
        {
            case 'S':
                mySchemes.back()->insertParameter(tokens.at(index-1)->getDescription());
                break;
            case 'F':
                break;
            case 'R':
                myRules.back()->insertParameter(tokens.at(index-1)->getDescription());
                break;
            case 'Q':
                break;
            case 'D':
                break;
            default:
                cout << endl << "ERROR: Flag variable not defined!";
        }
        //idList
        idList();
        return;
        }
        else if (tokens.at(index)->getMyTypeToPrint() == "RIGHT_PAREN")//RIGHT_PAREN is in the follow set of idlist
        {
            return; //just pop, do not advnace the input.
        }
        else
        {
            throw tokens.at(index);
        }
    }

    void headPredicate ()
    {
        string ruleToHeadPredicate;
        //ID LEFT_PAREN ID idList RIGHT_PAREN
        //ID
        match (tokens.at(index), "ID"); //ID is in the first set of ID
        ruleToHeadPredicate = ruleToHeadPredicate + tokens.at(index-1)->getDescription();
        //LEFT_PAREN
        match (tokens.at(index), "LEFT_PAREN"); //LEFT_PAREN is in the first set of LEFT_PAREN

        myRules.push_back(new Rule(ruleToHeadPredicate));
        //ID
        match (tokens.at(index), "ID"); //ID is in the first set of ID
        myRules.back()->insertParameter(tokens.at(index-1)->getDescription());
        //idList
        idList();
        //RIGHT_PAREN
        match (tokens.at(index), "RIGHT_PAREN"); //RIGHT_PAREN is in the first set of RIGHT_PAREN
    }

    void parameter ()
    {
        //STRING | ID
        if (tokens.at(index)->getMyTypeToPrint() == "STRING")//STRING is in the first set of parameter
        {
            //STRING
            match (tokens.at(index), "STRING"); //STRING is in the first set of STRING
            switch (flag)
            {
                case 'R':
                    myRules.back()->insertParameterIntoPredicate(tokens.at(index-1)->getDescription());
                    break;
                case 'Q':
                    myQueries.back()->insertParameter(tokens.at(index-1)->getDescription());
                    break;
                default:
                    cout << endl << "ERROR: Flag variable not defined!";
            }
            return;
        }
        else if (tokens.at(index)->getMyTypeToPrint() == "ID")//ID is in the first set of parameter
        {
            //ID
            match (tokens.at(index), "ID"); //ID is in the first set of ID
            switch (flag)
            {
                case 'R':
                    myRules.back()->insertParameterIntoPredicate(tokens.at(index-1)->getDescription());
                    break;
                case 'Q':
                    myQueries.back()->insertParameter(tokens.at(index-1)->getDescription());
                    break;
                default:
                    cout << endl << "ERROR: Flag variable not defined!";
            }
            return; //just pop, do not advance the input.
        }
        else
        {
            throw tokens.at(index);
        }
    }

    void parameterList ()
    {
        //COMMA parameter parameterList | lambda
        if (tokens.at(index)->getMyTypeToPrint() == "COMMA")//COMMA is in the first set of parameterList
        {
            //COMMA
            match (tokens.at(index), "COMMA"); //COMMA is in the first set of COMMA
            //parameter
            parameter();
            //parameterList
            parameterList();
            return;
        }
        else if (tokens.at(index)->getMyTypeToPrint() == "RIGHT_PAREN")//PERIOD is in the follow set of parameterList
        {
            return; //just pop, do not advance the input.
        }
        else
        {
            throw tokens.at(index);
        }
    }

    void predicate ()
    {
        string queryToPredicate;
        //ID LEFT_PAREN parameter parameterList RIGHT_PAREN
        //ID
        match (tokens.at(index), "ID"); //ID is in the first set of ID
        queryToPredicate = queryToPredicate + tokens.at(index-1)->getDescription();
        //LEFT_PAREN
        match (tokens.at(index), "LEFT_PAREN"); //LEFT_PAREN is in the first set of LEFT_PAREN

        switch (flag)
        {
            case 'R':
                myRules.back()->insertPredicate(queryToPredicate);
                break;
            case 'Q':
                myQueries.push_back(new Predicate(queryToPredicate)); //makes a query from the line.
                break;
            default:
                cout << endl << "ERROR: Flag variable not defined!";
        }
        //parameter
        parameter();
        //parameterList
        parameterList();
        //RIGHT_PAREN
        match (tokens.at(index), "RIGHT_PAREN"); //RIGHT_PAREN is in the first set of RIGHT_PAREN
    }

    void predicateList ()
    {
        //COMMA predicate predicateList | lambda
        if (tokens.at(index)->getMyTypeToPrint() == "COMMA")//COMMA is in the first set of predicateList
        {
            //COMMA
            match (tokens.at(index), "COMMA"); //COMMA is in the first set of COMMA
            //predicate
            predicate();
            //predicateList
            predicateList();
            return;
        }
        else if (tokens.at(index)->getMyTypeToPrint() == "PERIOD")//PERIOD is in the follow set of predicateList
        {
            return; //just pop, do not advance the input.
        }
        else
        {
            throw tokens.at(index);
        }
    }

    void rule ()
    {
        //headPredicate COLON_DASH predicate predicateList PERIOD
        //headPredicate
        headPredicate ();
        //COLON_DASH
        match (tokens.at(index), "COLON_DASH"); //LEFT_PAREN is in the first set of LEFT_PAREN
        //predicate
        predicate ();
        //predicateList
        predicateList ();
        //PERIOD
        match (tokens.at(index), "PERIOD"); //RIGHT_PAREN is in the first set of RIGHT_PAREN
    }

    void ruleList ()
    {
        //rule ruleList | lambda
        if (tokens.at(index)->getMyTypeToPrint() == "ID")//ID is in the first set of ruleList
        {
            //rule
            rule();
            //ruleList
            ruleList();
            return;
        }
        else if (tokens.at(index)->getMyTypeToPrint() == "QUERIES")//FACTS is in the follow set of ruleList
        {
            return; //just pop, do not advance the input.
        }
        else
        {
            throw tokens.at(index);
        }
    }

    void scheme ()
    {
        string schemeToPredicate;
        //ID LEFT_PAREN ID idList RIGHT_PAREN
        //ID
        match (tokens.at(index), "ID"); //ID is in the first set of ID
        schemeToPredicate = schemeToPredicate + tokens.at(index-1)->getDescription();
        //LEFT_PAREN
        match (tokens.at(index), "LEFT_PAREN"); //LEFT_PAREN is in the first set of LEFT_PAREN

        mySchemes.push_back(new Predicate(schemeToPredicate)); //makes a scheme from the line.
        //ID
        match (tokens.at(index), "ID"); //ID is in the first set of ID
        mySchemes.back()->insertParameter(tokens.at(index-1)->getDescription());
        //idlist
        idList ();
        //RIGHT_PAREN
        match (tokens.at(index), "RIGHT_PAREN"); //RIGHT_PAREN is in the first set of RIGHT_PAREN

    }

    void schemeList ()
    {
        //scheme schemeList | lambda
        if (tokens.at(index)->getMyTypeToPrint() == "ID")//ID is in the first set of schemeList
        {
            //scheme
            scheme();
            //schemeList
            schemeList();
            return;
        }
        else if (tokens.at(index)->getMyTypeToPrint() == "FACTS")//FACTS is in the follow set of schemeList
        {
            return; //just pop, do not advnace the input.
        }
        else
        {
            throw tokens.at(index);
        }
    }

    void stringList ()
    {
        //COMMA STRING stringList | lambda
        if (tokens.at(index)->getMyTypeToPrint() == "COMMA")//COMMA is in the first set of stringList
        {
            //COMMA
            match (tokens.at(index), "COMMA"); //COMMA is in the first set of COMMA
            //ID
            match (tokens.at(index), "STRING"); //STRING is in the first set of STRING
            if (flag == 'D')
            {
                string stringToInsert = tokens.at(index-1)->getDescription();///myDomain
                myDomain.insert(stringToInsert);///myDomain
                myFacts.back()->insertParameter(tokens.at(index-1)->getDescription());///myFacts
            }
            //stringList
            stringList();
            return;
        }
        else if (tokens.at(index)->getMyTypeToPrint() == "RIGHT_PAREN")//RIGHT_PAREN is in the follow set of stringList
        {
            return; //just pop, do not advance the input.
        }
        else
        {
            throw tokens.at(index);
        }
    }

    void fact ()
    {
        string factToPredicate;
        //ID LEFT_PAREN STRING stringList RIGHT_PAREN PERIOD
        //ID
        match (tokens.at(index), "ID"); //ID is in the first set of ID
        factToPredicate = factToPredicate + tokens.at(index-1)->getDescription();
        //LEFT_PAREN
        match (tokens.at(index), "LEFT_PAREN"); //LEFT_PAREN is in the first set of LEFT_PAREN

        myFacts.push_back(new Predicate(factToPredicate)); //makes a fact from the line.
        //STRING
        flag = 'D'; ///Changes to Domain mode
        match (tokens.at(index), "STRING"); //STRING is in the first set of STRING

        //Every fact has at least one string, so I will insert it into the set of strings
        // into my list of parameters for the vector myFacts.
        string stringToInsert = tokens.at(index-1)->getDescription();///myDomain
        myDomain.insert(stringToInsert);///myDomain
        myFacts.back()->insertParameter(tokens.at(index-1)->getDescription());///myFacts
        //Then I can go into my stringList funtion to possibly pick up more strings to insert

        //stringList
        stringList ();
        flag = 'F'; ///Changes back to Fact mode
        //RIGHT_PAREN
        match (tokens.at(index), "RIGHT_PAREN"); //RIGHT_PAREN is in the first set of RIGHT_PAREN
        //PERIOD
        match (tokens.at(index), "PERIOD"); //RIGHT_PAREN is in the first set of RIGHT_PAREN
    }

    void factList ()
    {
        //fact factList | lambda
        if (tokens.at(index)->getMyTypeToPrint() == "ID")//ID is in the first set of factList
        {
            //fact
            fact();
            //factList
            factList();
            return;
        }
        else if (tokens.at(index)->getMyTypeToPrint() == "RULES")//RULES is in the follow set of factList
        {
            return; //just pop, do not advance the input.
        }
        else
        {
            throw tokens.at(index);
        }
    }
};

///Class to parse out whatever kind of Program I've been given

class Parser {
private:
    DatalogProgram myProgram; //has a DatalogProgram object in it

public:
    Parser(vector<Token*>& lexerTokens) : myProgram(lexerTokens) {beginProgram (myProgram);};

    void beginProgram(DatalogProgram&);
    DatalogProgram& getProgram();
};

void Parser::beginProgram(DatalogProgram&)
{
    try {
        myProgram.begin();
        //myProgram.toString();
    }
    catch (Token* T)
    {
        cout << "Failure!" << endl << "  ";
        T->toString();
    }
}

DatalogProgram& Parser::getProgram()
{
    return myProgram;
}

#endif //PROJECT_0_PARSER_H
