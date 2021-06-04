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

    bool shouldContinue ()
    {
        bool keepRunning = false;
        for (map<string, Relation>::iterator it = myRelations.begin(); it != myRelations.end(); it++)//for every relation in the map
        {
            if (it->second.getStatus()) { //if it finds a relation that has a tuple added
                //cout << endl << "It should keep running!";
                keepRunning = true; //that means it should keep running
                boolReset(); //resets all the relations back to their initial value of false
                break;
            }
        }
        return keepRunning;
    }

    void boolReset ()
    {
        for (map<string, Relation>::iterator it = myRelations.begin(); it != myRelations.end(); it++)//for every relation in the map
        {
            it->second.resetStatus(); //Reset all the keepRunning members to false
        }
    }

    void toString ()
    {
        for (map<string, Relation>::iterator it = myRelations.begin(); it != myRelations.end(); it++)//for every relation in the map
        {
            cout << endl << endl << "MY RELATION: " << it->second.getName();
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

        //cout << endl << "-------------What's already inside----------------";
        //myDatabase.toString();
        ///this is a function to test
        //joinWithDatabase();

        //cout << endl << endl << "-------------Start Rules----------------";
        cout << "Rule Evaluation";
        EvaluateRules();

        //cout << endl << endl << "-------------Queries Below----------------" << endl;
        cout << endl << endl << "Query Evaluation" << endl;
        EvaluateQueries();

    }

    void EvaluateRules () ///Fixed Point Algorithm
    {
        /** Does Fancy Rule Evaluation :) */
        int numberOfPasses = 0;
        bool keepRunning;
        do
        {
            ++numberOfPasses;
            for (size_t i = 0; i < myProgramToInterpret.getRules().size(); ++i) //for every Rule in the program's Rule vector
            {
//                if (i != 0)
//                {
//                    cout << endl;
//                }
                //cout << "Rule Number: " << i+1;
                myProgramToInterpret.getRules().at(i)->toString(); //Print out the Rule we are looking at.

                EvaluateRule(*myProgramToInterpret.getRules().at(i));
            }

            keepRunning = false;

            if (myDatabase.shouldContinue()) //if the Database finds a tuple added
            {
                keepRunning = true; //it will continue the loop
            }
        }
        while (keepRunning);

        cout << endl << endl << "Schemes populated after " << numberOfPasses << " passes through the Rules.";
    }

    void EvaluateRule (Rule& inputRule)
    {
        ///Step 1: Evaluate the RHS
        vector<Relation> results;
        for (size_t i = 0; i < inputRule.getPredicate().size(); ++i) //for every Predicate in the RHS of the Rule
        {
            Relation solutionRelation = EvaluatePredicate(*inputRule.getPredicate().at(i));
            results.push_back(solutionRelation); //Push it back into the list of results
        }

        //cout << endl << "            Each Predicate Solution            ";
//        for (Relation r : results)
//        {
//            r.toString();
//        }

        ///Step 2: Join them all together
        Relation joinedResult = results.front();
        if (results.size() > 1) //if we need to join two or more relations (i.e. there is more than one relation)
        {
            for (size_t i = 0; i < results.size()-1; ++i) // for every relation in the results (except the last one)
            {
                //cout << endl << "We joined two relations!";
                joinedResult = joinedResult.Join(results.at(i+1));
                //cout << endl << "Result after Join:";
                //joinedResult.toString();
            }
        }

        ///Step 3: Project just the columns that are union worthy
        vector<string> parametersToMatch;
        vector<int> indexesToMatch;
        for (size_t i = 0; i < inputRule.getHeadParameters().size(); ++i) //for every parameter in the head predicate
        {
            string input;
            input = inputRule.getHeadParameters().at(i)->getDescription(); //get that parameter's description
            parametersToMatch.push_back(input);                               //and place it into the vector of strings
        }

        indexesToMatch = joinedResult.FindMatchingIndexVector(parametersToMatch); //get the indexes needed for the project
        joinedResult.Project(indexesToMatch); //do the project with the given indexes

        //cout << endl << "Result after Project:";
        //joinedResult.toString();

        ///Step 4: Rename to match the corresponding scheme
        //FIXME: Could be inefficient?
        string currentScheme = inputRule.getDescription(); //gets the name of this rule
        Header desiredHeader = myDatabase.getMap().at(currentScheme).getHeader(); //finds the corresponding Header
        vector<string> desiredStringVector = desiredHeader.getVector(); //makes a vector of strings out of it
        joinedResult.Rename(desiredStringVector);

        //cout << endl << "Result after Rename:";
        //joinedResult.toString();

        ///Step 5: Union with Database
        Relation& databaseRelation = myDatabase.getMap().at(currentScheme);
        databaseRelation.UnionTuplesFrom(joinedResult);
        //cout << endl << "Database Result after Union:";
        //databaseRelation.toString();
    }

    void joinWithDatabase () ///This is a Test
    {
        cout << endl << "_________NOW TESTING Join: Made my own relation_________";

        Header header1;
        header1.addAttributeToHeader("C");
        header1.addAttributeToHeader("S");
        header1.addAttributeToHeader("G");

        Tuple first;
        first.addValueToTuple("\'CS101\'");
        first.addValueToTuple("\'12345\'");
        first.addValueToTuple("\'A\'");
        first.setHeader(header1);

        Tuple second;
        second.addValueToTuple("\'EE200\'");
        second.addValueToTuple("\'28282\'");
        second.addValueToTuple("\'B\'");
        second.setHeader(header1);

        string toInsert = "First Relation";
        Relation myRelation1(toInsert, header1);
        myRelation1.addTuple(first);
        myRelation1.addTuple(second);
        myRelation1.toString();
        //cout << endl << "_________Print Created Relation's Header_________"<< endl;
        //myRelation1.getHeader().toString();

        Header header2;
        header2.addAttributeToHeader("c");
        header2.addAttributeToHeader("s");
        header2.addAttributeToHeader("g");

        toInsert = "Second Relation";
        Relation myRelation2(toInsert, header2);
        myRelation2.addTuple(first);
        myRelation2.addTuple(second);
        //myRelation2.toString();

        Relation& UnionRelation = myDatabase.getMap().at("csg");
        cout << endl << "_________Other relation to join with:_________" << endl;
        UnionRelation.toString();
        //UnionRelation.getHeader().toString();

        UnionRelation = UnionRelation.Join(myRelation1);
        cout << endl << "_________After join:_________"<< endl;
        UnionRelation.toString();



        /*cout << endl << "Needs To rerun? Answer: " ;
        if (UnionRelation.getStatus())
        {
            cout << "Yes";
        }
        else { cout << "No";}*/

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
