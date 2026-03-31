#ifndef LEXER_H
#define LEXER_H

#include "Token.h"
#include <string>
#include <vector>
#include "../include/Tools.h"

using namespace std;

class Lexer {
  vector<Token> tokens;
  string SourceCode;
public:
  explicit Lexer(std::string sourceCode);
  vector<Token> GetTokens();
  vector<Token> Tokenizer();
static Token initialToken(const std::string& word);
  Token& operator[](int i);



  int size() const;
};

#endif // LEXER_H