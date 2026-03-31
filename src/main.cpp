#include "Interpreter.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "Lexer.h"
#include "Parser.h"

using namespace std;

int main(int argc, char *argv[])
{
    // argc is the number of arguments, argv[0] is the program name, argv[1] is the first parameter (filename)
    if (argc < 2) {
        cerr << "Error: Please specify a file to run." << endl;
        cerr << "Usage: " << argv[0] << " [filename.mlg]" << endl;
        return 1;
    }

    // 2. Get the file path from argv[1]
    string path = argv[1];

    ifstream MyFile(path);
    string Code;

    if (MyFile.is_open()) {
        stringstream buffer;
        buffer << MyFile.rdbuf();
        Code = buffer.str();
        MyFile.close();
    }
    else {
        cerr << "Error: Unable to open file: " << path << endl;
        return 1;
    }

    if (Code.empty()) {
        cerr << "Warning: The file is empty!" << endl;
        return 1;
    }

    // Pass the code read from the file instead of static text
    Lexer MyLexer(Code);
    MyLexer.Tokenizer();
    Parser MyParser(MyLexer);
    auto p = MyParser.Parsering();
    interpreter MyInterpreter(std::move(p));
    MyInterpreter.run();
    
    system("pause");

    return 0;
}