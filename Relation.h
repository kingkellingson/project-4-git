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
    bool needsToRerun = false;
public:
    Relation (string& n, Header& h) : name(n), myHeader(h) {}

    string getName ()
    {
        return name;
    }

    bool getStatus ()
    {
        return needsToRerun;
    }

    void resetStatus ()
    {
        needsToRerun = false;
    }

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

    set<Tuple> GetSetOfTuples ()
    {
        return myTuples;
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

    void UnionTuplesFrom (Relation& inputRelation)
    {
        if (myHeader.getVector() != inputRelation.getHeader().getVector())
        {
            cout << "ERROR: The two Relations do not have matching Headers!";
            return;
        }

        for (Tuple t : inputRelation.GetSetOfTuples()) //for every Tuple in the passed set of tuples
        {
            if(myTuples.insert(t).second) //inserts it into the parent set of Tuples and checks to see if it was unique
            {
                t.toString();
                needsToRerun = true;
            }
        }
    }

    Relation Join (Relation& toJoinWith)
    {

        bool doCrossProduct = false;
        ///These are the indexes where the columns of the two relations should match up
        vector <int> firstRelationMatches;
        vector <int> secondRelationMatches;

        Header joinedHeader = combineHeaders (myHeader, toJoinWith.getHeader(), doCrossProduct, firstRelationMatches, secondRelationMatches);

//        ///This is a test for if the Cross Product should be run
//        if (doCrossProduct)
//        {
//            cout << endl << "$$$$$$$$$$$We're Doing Cross Product Now!$$$$$$$$$$$";
//        }

        string myName = "MyNewHeader";
        Relation joinedRelation (myName, joinedHeader);

        for (Tuple t1 : myTuples)
        {
            for (Tuple t2 : toJoinWith.GetSetOfTuples())
            {
                if (isJoinable(t1,firstRelationMatches, t2, secondRelationMatches) || doCrossProduct)
                {
                    ///Perhaps adding a if statement for when a cross product should be done?
                    //cout << endl << "*****Either CrossProduct or join the Tuples!******";
                    Tuple joinedTuple = combineTuples(t1, t2, secondRelationMatches);
                    joinedTuple.setHeader(joinedHeader);
                    joinedRelation.addTuple(joinedTuple);
                }
                else
                {
                    //cout << endl << "^^^^^Not possible to combine these Tuples :(^^^^^";
                }
            }
        }

        return joinedRelation;
    }

    bool isJoinable (Tuple T1, vector<int>& index1, Tuple T2, vector<int>& index2)
    {
        bool isJoinable = true;
        for (size_t i = 0; i < index1.size(); ++i)
        {
            ///This gets the values at the indexes where the two tuples should match
            string value1 = T1.getVector().at(index1.at(i));
            string value2 = T2.getVector().at(index2.at(i));
            if (value1 == value2) {continue;} //if the two values are equal, continue checking //FIXME: Potential error here with the if else
            else
            {
                isJoinable = false; //if not, then you cannot join the tuples
            }
        }
        return isJoinable;
    }

    Tuple combineTuples (Tuple T1, Tuple T2, vector<int>& indexToSkip)
    {
        Tuple myTuple;
        vector<string> newValues = T1.getVector(); //dump in all the values from the first tuple's vector of values

        for (size_t i = 0; i < T2.getVector().size(); ++i) //go through every item in the SECOND tuple
        {
            ///This just all checks to see if it should be skipped (i.e. we should add something else
            bool skip = false;
            for (size_t j = 0; j < indexToSkip.size(); ++j)
            {
                int column = i;
                if (column == indexToSkip.at(j)) // if the current collumn is also one of the columns to skip
                {
                    skip = true;
                }
            }
            if (skip) //if this index should be skipped
            {
                skip = false; //reset back to false
                continue; //then goe onto the next iteration.
            }

            newValues.push_back(T2.getVector().at(i)); //then insert the item in the second tuple
        }
        myTuple.setVector(newValues); //set the final product of the value vector

        return myTuple;
    }

    Header combineHeaders (Header h1, Header h2, bool& doCrossProduct, vector<int>& relation1, vector<int>& relation2)
    {
        Header myHeader;
        vector<string> newAttributes = h1.getVector();
        doCrossProduct = true; //I'm going to assume that I'll be doing a cross product unless the bool gets changed
                                // (i.e. something in header2 was already in the header1, meaning that there was overlap)

        for (size_t i = 0; i < h2.getVector().size(); ++i) //go through every item in the SECOND header
        {
            bool alreadyIn = false;
            for (size_t j = 0; j < h1.getVector().size(); ++j) //go through every item in the FIRST header (which was already added)
            {
                if (h2.getVector().at(i) == h1.getVector().at(j)) //if there is a match in any of the two headers
                {
                    alreadyIn = true; //then it is already in the header;
                    doCrossProduct = false;
                    ///found the column where there should be a match, so adding it to both vectors
                    relation1.push_back(j);
                    relation2.push_back(i);
                }
            }
            if (!alreadyIn) //if it is not already in the header
            {
                newAttributes.push_back(h2.getVector().at(i)); //then insert the item in the second header
            }
        }
        myHeader.setVector(newAttributes); //set the final product of the attribute vector
        return myHeader;
    }

    vector<int> FindMatchingIndexVector (vector<string>& stringsToMatch)
    {
        vector<int> indexesToSave;
        for (size_t i = 0; i < stringsToMatch.size(); ++i) // for every string in the strings to match
        {
            for (size_t j = 0; j < myHeader.getVector().size(); ++j) //go through every string in the relation's header
            {
                if (stringsToMatch.at(i) == myHeader.getVector().at(j)) //if they match
                {
                    indexesToSave.push_back(j); //add that index to the indexes needed to save
                    break;
                }
            }
        }
        return indexesToSave;
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
            if (t.getVector().size()==0)
            {
                continue;
            }
            t.toString();
        }
    }
};

#endif //PROJECT_0_RELATION_H
