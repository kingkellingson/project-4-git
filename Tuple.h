//
// Created by Kyle Ellingson on 5/26/21.
//

#ifndef PROJECT_0_TUPLE_H
#define PROJECT_0_TUPLE_H

#include <vector>
#include <set>
#include <iterator>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

class Header {
private:
    vector<string> attributes;
public:
    Header (vector<string> input) : attributes (input) {}
    Header () {}

    void addAttributeToHeader (string valueToAdd)
    {
        attributes.push_back(valueToAdd);
    }

    vector<string> getVector () const
    {
        return attributes;
    }

    void toString ()
    {
        for (size_t i = 0; i < attributes.size(); ++i)
        {
            if (i == 0)
            {
                cout << attributes.at(i);
            }
            else
            {
                cout << "," << attributes.at(i);
            }
        }
    }

    int size ()
    {
        return attributes.size();
    }

    void toString (int index)
    {
        if (attributes.size() == 0)
        {
            cout << endl << "ERROR: Trying to print an Empty Header!";
            return;
        }
        cout << attributes.at(index);
    }
};

class Tuple {
private:
    vector<string> values;
    Header header;
public:
    bool operator< (const Tuple& other) const
    {
        if (this->getVector() < other.getVector())
        {
            return true;
        }
        else return false;
    }


    void setHeader (Header input)
    {
        header = input;
    }

    Header getHeader ()
    {
        return header;
    }

    vector<string> getVector () const
    {
        return values;
    }

    void addValueToTuple (string valueToAdd)
    {
        values.push_back(valueToAdd);
    }

    void toString ()
    {
        cout << endl << "  ";
        for (size_t i = 0; i < values.size(); ++i)
        {
            header.toString(i);
            cout << "=" << values.at(i);
            if (i == values.size()-1) {}
            else
            {
                cout << ", ";
            }
        }
    }

    void toString (int index)
    {
        cout << values.at(index);
    }
};

#endif //PROJECT_0_TUPLE_H
