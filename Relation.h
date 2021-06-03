//
// Created by Kyle Ellingson on 5/26/21.
//

#ifndef PROJECT_0_RELATION_H
#define PROJECT_0_RELATION_H

#include <vector>
#include <set>
#include <iterator>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

class Relation {
private:
    string name;
    Header myHeader;
    set<Tuple> myTuples;
public:
    Relation (string& n, Header& h) : name(n), myHeader(h) {}

    void addTuple (Tuple tupleToAdd)
    {
        myTuples.insert(tupleToAdd);
    }

    void SetHeader(Header input)
    {
        myHeader = input;
    }

    void SetSetOfTuples (set<Tuple> input)
    {
        myTuples = input;
    }

    Relation ConstantSelect (string toMatch, size_t index)
    {
        set<Tuple> newRelationSet; //the set that I want to return

        for (Tuple t : myTuples) //go through and find the matches
        {
            if (t.getVector().at(index) == toMatch)
            {
                newRelationSet.insert(t);
                //t.setToKeep();
            }
        }
        this->SetSetOfTuples(newRelationSet);
        return *this;
    }

    Relation VariableSelect (size_t index1, size_t index2)
    {
        set<Tuple> newRelationSet; //the set that I want to return

        for (Tuple t : myTuples) //go through and find the matches
        {
            if (t.getVector().at(index1) == t.getVector().at(index2)) //if the two match each other
            {
                newRelationSet.insert(t); //then insert it into the new set
            }
        }
        this->SetSetOfTuples(newRelationSet);
        return *this;
    }

    Relation Project (vector<int> columnsToKeep)
    {
        ///This all deals with the Header
        Header newHeader;
        for (size_t i = 0; i < columnsToKeep.size(); ++i) //for every index in the index vector
        {
            int attributeToKeep = columnsToKeep.at(i); //get the desired index.
            string valueToAdd = myHeader.getVector().at(attributeToKeep); // get the string at that index
            newHeader.addAttributeToHeader(valueToAdd); //put it into a new header
        }

        ///This all deals with the Tuples
        set<Tuple> newRelationSet;
        for (Tuple t : myTuples) //for every tuple
        {
            Tuple newTuple; //creates the desired Tuple
            for (size_t i = 0; i < columnsToKeep.size(); ++i) //for every index in the index vector
            {
                int columnToKeep = columnsToKeep.at(i); //get the desired index.
                string valueToAdd = t.getVector().at(columnToKeep); // get the string at that index
                newTuple.addValueToTuple(valueToAdd); //put it into a new tuple FIXME: Add a header
                newTuple.setHeader(newHeader);
            }
            newRelationSet.insert(newTuple); //new Tuple is finished, so put it in the new Set.
        }

        this->SetHeader(newHeader);
        this->SetSetOfTuples(newRelationSet);
        return *this;
    }

    Relation Rename (vector<string>& attributesToRename)
    {
        ///This all deals with the Header
        Header newHeader;
        for (size_t i = 0; i < attributesToRename.size(); ++i) //for every name in the name vector
        {
            string attributeToRename = attributesToRename.at(i); //get the desired name.
            newHeader.addAttributeToHeader(attributeToRename); //put it into a new header
        }

        set<Tuple> newRelationSet;
        for (Tuple t : myTuples) //for every tuple
        {
            Tuple newTuple; //creates the desired Tuple
            newTuple = t;
            newTuple.setHeader(newHeader);
            newRelationSet.insert(newTuple); //new Tuple is finished, so put it in the new Set.
        }

        this->SetHeader(newHeader);
        this->SetSetOfTuples(newRelationSet);
        return *this;
    }

    string getName ()
    {
        return name;
    }

    int NumberTuples ()
    {
        return myTuples.size();
    }

    Header getHeader ()
    {
        return myHeader;
    }

    void toString ()
    {
        for (Tuple t : myTuples)
        {
            t.toString();
        }
    }
};

#endif //PROJECT_0_RELATION_H
