//
// Created by Kyle Ellingson on 5/13/21.
//

#ifndef PROJECT_0_DATASTRUCTURES_H
#define PROJECT_0_DATASTRUCTURES_H

class Parameter
{
private:
    string description = "";
public:
    Parameter() {}
    Parameter(string s) : description(s) {}
    void toString () const {
        cout << description;
    }
    string getDescription ()
    {
        return description;
    }
};

class Predicate
{
private:
vector<Parameter*> myParameters;
string description;
public:
    Predicate(string s) : description(s) {}
    void toString ()
    {
        cout << description << "(";
        for (size_t i = 0; i < myParameters.size(); ++i)
        {
            if (i != 0 )
            {
                cout << ",";
            }
                myParameters.at(i)->toString();
        }
        cout << ")";
    }

    string getDescription ()
    {
        return description;
    }

    vector<Parameter*> getParameters ()
    {
        return myParameters;
    }

    void insertParameter (string toInsert)
    {
        myParameters.push_back(new Parameter(toInsert));
    }
};

class Rule
{
private:
    vector<Predicate*> myPredicates;
    vector<Parameter*> myParameters;
    string description;
public:
    Rule(string s) : description(s) {}
    void toString ()
    {
        cout << endl << "  " << description;
        for (size_t i = 0; i < myParameters.size(); ++i)
        {
            if (i != 0 )
            {
                cout << ",";
            }
            myParameters.at(i)->toString();
        }
        cout << ") :- ";
        for (size_t i = 0; i < myPredicates.size(); ++i)
        {
            if (i != 0 )
            {
                cout << ",";
            }
            myPredicates.at(i)->toString();
        }
    }

    void insertParameter (string toInsert)
    {
        myParameters.push_back(new Parameter(toInsert));
    }

    void insertPredicate (string toInsert)
    {
        myPredicates.push_back(new Predicate(toInsert));
    }

    void insertParameterIntoPredicate (string toInsert)
    {
        myPredicates.back()->insertParameter(toInsert);
    }
};

#endif //PROJECT_0_DATASTRUCTURES_H

/*
switch (flag)
{
    case 'S':
        break;
    case 'F':
        break;
    case 'R':
        break;
    case 'Q':
        break;
    case 'D':
        break;
    default:
        cout << endl << "ERROR: Flag variable not defined!";
}
*/