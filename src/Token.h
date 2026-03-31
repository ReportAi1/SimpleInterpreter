//
// Created by 12HP23 on 17/03/2026.
//

#ifndef SIMPLEINTERPRETER_TOKEN_H
#define SIMPLEINTERPRETER_TOKEN_H

#include <string>
#include "iostream"

using namespace std;

enum class Tokentype{
  PRINT,
  INPUT,
  LET,
  IDENTIFIER,
  NUMBER,
  STRING,
  BOOL,
  CHAR_LITERAL,
  OPERATOR,
  COMMENT,
  SEMICOLON,
  LEFT_PAREN,
  RIGHT_PAREN,
  LEFT_BRACKET,
  RIGHT_BRACKET,
  LEFT_BRACE,
  RIGHT_BRACE,
  END_OF_FILE
};

struct Token
{
  Tokentype type;
  std::string Value;
public:

  Token(Tokentype type, const char * str): type(type), Value(str){}
  Token(): type(Tokentype::END_OF_FILE), Value("U"){}
  Tokentype GetType();

  std::string GetValue();
};
inline Tokentype Token::GetType() { return this->type; }
inline std::string Token::GetValue() { return this->Value; }

#endif // SIMPLEINTERPRETER_TOKEN_H
