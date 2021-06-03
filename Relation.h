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

        Header joinedHeader = combineHeaders (myHeader, toJoinWith.getHeader());

        string myName = "MyNewHeader";
        Relation joinedRelation (myName, joinedHeader);

        /*for (Tuple t1 : myTuples)
        {
            for (Tuple t2 : toJoinWith.GetSetOfTuples())
            {
                if (isJoinable(t1, t2))
                {
                    Tuple joinedTuple = combineTuples(t1, t2);
                    joinedRelation.addTuple(joinedTuple);
                }
            }
        }*/

        return joinedRelation;

        /*
         * 	make the header h for the result relation
	    (combine r1's header with r2's header)

	make a new empty relation r using header h

	for each tuple t1 in r1
	    for each tuple t2 in r2

		if t1 and t2 can join
		    join t1 and t2 to make tuple t
		    add tuple t to relation r
		end if

	    end for
	end for
         */
    }

    Header combineHeaders (Header h1, Header h2)
    {
        Header myHeader;
        vector<string> newAttributes = h1.getVector();

        for (size_t i = 0; i < h2.getVector().size(); ++i) //go through every item in the second header
        {
            bool alreadyIn = false;
            for (size_t j = 0; j < h1.getVector().size(); ++j) //go through every item in the first header (which was already added)
            {
                if (h2.getVector().at(i) == h1.getVector().at(j)) //if there is a match in any of the two headers
                {
                    alreadyIn = true; //then it is already in the header;
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

    bool isJoinable (Tuple T1, Tuple T2)
    {
        return true;
    }

    Tuple combineTuples (Tuple T1, Tuple T2)
    {
        Tuple myTuple;
        return myTuple;
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
