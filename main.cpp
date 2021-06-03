#include "Lexer.h"
#include "Parser.h"
#include "Interpreter.h"
#include "Token.h"
#include <iostream>
#include <istream>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <sstream>


using std::cout;
using std::string;
using std::ostream;
using std::iostream;
using std::istream;
using std::ifstream;
using std::ofstream;
using std::endl;
using std::cin;
using std::istringstream;


int main(int argc, char* argv[]) {

    // open argv[1] or source file for input
    istream& in = *(new ifstream(argc > 1 ? argv[1] : "input.txt"));
    if (!in) return 1;

    // open argv[2] or cout for output
    ostream& out = (argc > 2) ? *(new ofstream(argv[2])) : cout;
    if (!out) return 2;

    string inputString = "";
    for (string line; getline(in, line);)
    {
        //if (line.size() == 0) continue;
        inputString = inputString + line + "0endl0";
    }
    inputString = inputString + "EOF_TYPE";

    ///Project One:

    Lexer* lexer = new Lexer(inputString);

    ///Project Two:

    vector<Token*> lexerTokens = lexer->getVectorOfTokens();
    Parser* parser = new Parser (lexerTokens);

    ///Project Three:

    DatalogProgram parserOutput = parser->getProgram();
    Interpreter* interpreter = new Interpreter (parserOutput);

    // TODO

    delete lexer;
    delete parser;
    delete interpreter;
    //cout << endl << ;"End of Program!"
    return 0;
}