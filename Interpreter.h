//
// Created by Kyle Ellingson on 5/26/21.
//

#ifndef PROJECT_0_INTERPRETER_H
#define PROJECT_0_INTERPRETER_H

#include <vector>
#include <set>
#include <map>
#include <iterator>
#include <iostream>
#include <sstream>
#include "Token.h"
#include "Tuple.h"
#include "Relation.h"
#include "Parser.h"
#include "DataStructures.h"

using namespace std;

///Class to hold all of the relations in a Map of strings to Relations

class Database {
private:
    map<string, Relation> myRelations;
public:
    void addRelationToMap (Relation& relationToAdd)
    {
        myRelations.insert(pair<string,Relation> (relationToAdd.getName(), relationToAdd));
    }

    map<string, Relation>& getMap ()
    {
        return myRelations;
    }

    void toString ()
    {
        for (map<string, Relation>::iterator it = myRelations.begin(); it != myRelations.end(); it++)
        {
            cout << endl << endl << "MY RELATION:";
            it->second.toString();
        }
    }
};

///Class to Interpret the vectors of Schemes, Rules, Facts, and Queries from the DatalogProgram the Parser created.

class Interpreter {
private:
    DatalogProgram myProgramToInterpret; //has a DatalogProgram object in it
    Database myDatabase;
public:
    Interpreter (DatalogProgram& parserOutput) : myProgramToInterpret (parserOutput)
    {
        run();
    }

    void run ()
    {
        CreateRelations(); //Creates the Database according to the Schemes
        FillFacts(); //Fills the Relations with Tuples

        EvaluateQueries();
        //myDatabase.toString();
    }

    Relation EvaluatePredicate(Predicate predicate)
    {
        string nameToMatch = predicate.getDescription(); //Determines the name of the desired Relation
        Relation copyRelation = myDatabase.getMap().at(nameToMatch); //creates a copy of Relation that matched the name

        map <string, int> VariableMap;
        vector<string> VariablesNamesForRename;
        vector<int> VariableIndeciesForProject;



        ///SELECT functions
        for(size_t index = 0; index<predicate.getParameters().size(); ++index) //for every parameter in the predicate list.
        {
            string toMatch = predicate.getParameters().at(index)->getDescription(); //copies a string of the parameter in that position
            if (toMatch.at(0) == '\'' ) //if the first character of the selected predicate is a \'
            {
                copyRelation.ConstantSelect(toMatch, index); //perform a Type 1 select on it
            }
            else //if not, then it must be a variable.
            {
                if (VariableMap.find(toMatch)==VariableMap.end()) //if that predicate is not in the list
                {
                    VariableMap.insert(pair<string, int>(toMatch,index)); //then add it to the list
                    VariablesNamesForRename.push_back(toMatch); //add to string vector for Rename
                    VariableIndeciesForProject.push_back(index); //add to int vector for Project
                }
                else //if it IS in the list already
                {
                    size_t firstOccurrenceIndex = VariableMap.at(toMatch); //Makes an index of the first occurrence
                    copyRelation.VariableSelect(firstOccurrenceIndex, index); //then perform a Type 2 select on it
                }
            }
        }

        ///PROJECT
        copyRelation.Project(VariableIndeciesForProject);


        ///RENAME
        copyRelation.Rename(VariablesNamesForRename);


        return copyRelation;
    }

    void EvaluateQueries ()
    {
        for (size_t i = 0; i < myProgramToInterpret.getQueries().size(); ++i) //for every Query in the program's Query vector
        {
            if (i != 0)
            {
                cout << endl;
            }

            myProgramToInterpret.getQueries().at(i)->toString(); //Print out the Query we are looking at.
            cout << "? ";

            Relation EvaluatedPredicate = EvaluatePredicate(*myProgramToInterpret.getQueries().at(i)); //Make a Relation that is evaluated

            if (EvaluatedPredicate.NumberTuples()==0) //Outputs "No" if it does not have any tuples
            {
                cout << "No";
            }
            else
            {
                cout << "Yes(" << EvaluatedPredicate.NumberTuples() << ")"; //And "Yes" with the number of Tuples that matched
                EvaluatedPredicate.toString(); //then prints them out
            }
        }
    }

    void FillFacts ()
    {
        for (size_t i = 0; i < myProgramToInterpret.getFacts().size(); ++i) //for every Fact in the program's Fact vector
        {
            string nameToMatch = myProgramToInterpret.getFacts().at(i)->getDescription(); //Determines the name of the desired Relation
            Tuple tuple = CreateTuple (myProgramToInterpret.getFacts().at(i)); //Creates a tuple from the Fact's parameters

            Relation& myRelation = myDatabase.getMap().at(nameToMatch); //creates an alias of the Relation that matched the name
            tuple.setHeader(myRelation.getHeader()); //sets the header pointer of that created Tuple.
            myRelation.addTuple(tuple); //Adds the created Tuple to that relation, into its set of Tuples.
        }
    }

    Tuple CreateTuple (Predicate* predicate)
    {
        Tuple newTuple; //makes a new tuple
        vector <Parameter*> parameters_ = predicate->getParameters(); //makes a vector of Parameter pointers from Predicate
        for (size_t i = 0; i < parameters_.size(); ++i) //for each pointer
        {
            newTuple.addValueToTuple(parameters_.at(i)->getDescription()); //add its description to the Tuple
        }
        return newTuple; //returns the new Tuple
    }

    void CreateRelations ()
    {
        for (size_t i = 0; i < myProgramToInterpret.getSchemes().size(); ++i) //for every Scheme in the program's Scheme vector
        {
            Header header = CreateHeader (myProgramToInterpret.getSchemes().at(i)); //create a header from the parameters
            string name = myProgramToInterpret.getSchemes().at(i)->getDescription(); //gets the name of the Relation

            Relation newRelation (name, header); //Creates a relation from the name and header
            myDatabase.addRelationToMap(newRelation); //adds it to the Database
        }
    }

    Header CreateHeader (Predicate* predicate)
    {
        Header newHeader; //makes a new header
        vector <Parameter*> parameters_ = predicate->getParameters(); //makes a vector of Parameter pointers from Predicate
        for (size_t i = 0; i < parameters_.size(); ++i) //for each pointer
        {
            newHeader.addAttributeToHeader(parameters_.at(i)->getDescription()); //add its description to the Header
        }
        return newHeader; //returns the new Header
    }

};

#endif //PROJECT_0_INTERPRETER_H
