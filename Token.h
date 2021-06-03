#ifndef TOKEN_H
#define TOKEN_H
#include <string>
using namespace std;
#define NUMBER_OF_STRING_TOKENS 13

enum class TokenType {
    COMMA,
    PERIOD,
    Q_MARK,
    LEFT_PAREN,
    RIGHT_PAREN,
    MULTIPLY,
    ADD,
    SCHEMES,
    FACTS,
    RULES,
    QUERIES,
    COLON,
    COLON_DASH,
    ID,
    STRING,
    COMMENT,
    UNDEFINED,
    EOF_TYPE,
    MATCHER
};

class Token
{
private:
    // TODO: add member variables for information needed by Token
    TokenType type;
    string description;
    string myTypeToPrint;
    int lineNumber;

public:
    Token()
    {
        this->type = TokenType::UNDEFINED;
        this->description = "No Description!";
        this -> lineNumber = -1;
        // TODO: initialize all member variables
    }

    Token(TokenType type, std::string description, int line)
    {
        this->type = type;
        this->description = description;
        this -> lineNumber = line;
        // TODO: initialize all member variables

        string typeToPrint;
        switch (this->type)
        {
            case TokenType::COMMA:
                typeToPrint ="COMMA";
                break;
            case TokenType::PERIOD:
                typeToPrint ="PERIOD";
                break;
            case TokenType::Q_MARK:
                typeToPrint ="Q_MARK";
                break;
            case TokenType::LEFT_PAREN:
                typeToPrint ="LEFT_PAREN";
                break;
            case TokenType::RIGHT_PAREN:
                typeToPrint ="RIGHT_PAREN";
                break;
            case TokenType::COLON:
                typeToPrint ="COLON";
                break;
            case TokenType::COLON_DASH:
                typeToPrint ="COLON_DASH";
                break;
            case TokenType::MULTIPLY:
                typeToPrint ="MULTIPLY";
                break;
            case TokenType::ADD:
                typeToPrint ="ADD";
                break;
            case TokenType::SCHEMES:
                typeToPrint ="SCHEMES";
                break;
            case TokenType::FACTS:
                typeToPrint ="FACTS";
                break;
            case TokenType::RULES:
                typeToPrint ="RULES";
                break;
            case TokenType::QUERIES:
                typeToPrint ="QUERIES";
                break;
            case TokenType::ID:
                typeToPrint ="ID";
                break;
            case TokenType::STRING:
                typeToPrint ="STRING";
                break;
            case TokenType::COMMENT:
                typeToPrint ="COMMENT";
                break;
            case TokenType::UNDEFINED:
                typeToPrint ="UNDEFINED";
                break;
            case TokenType::EOF_TYPE:
                typeToPrint ="EOF";
                break;
            case TokenType::MATCHER:
                typeToPrint ="ERROR: A MATCHER AUTOMATON WAS NOT SET CORRECTLY!";
                break;
            default:
                typeToPrint = "ERROR: NO ASSIGNED TYPE!";
        }
        myTypeToPrint = typeToPrint;

    }

    // TODO: add other needed methods

    void toString ()
    {
        cout << "(" << myTypeToPrint << ",\"" << description << "\"," << lineNumber << ")";
    }

    string getMyTypeToPrint ()
    {
        return myTypeToPrint;
    }

    string getDescription ()
    {
        return description;
    }

    int getLineNumber ()
    {
        return lineNumber;
    }

};

#endif // TOKEN_H

