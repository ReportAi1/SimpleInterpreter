#include "Lexer.h"
#include <utility>
#include "../include/Tools.h"
#include "Token.h"

Lexer::Lexer(std::string sourceCode) : SourceCode(std::move(sourceCode)) {
}

vector<Token> Lexer::GetTokens() {
    return tokens;
}

using tokentype = Tokentype;

vector<Token> Lexer::Tokenizer() {
    string t;
    for (int i = 0; i < SourceCode.size(); i++) {

        // Skip whitespace
        if (isspace(SourceCode[i])) { continue; }

        // --- Words and Identifiers ---
        if (isalpha(SourceCode[i])) {
            while (i < SourceCode.size() && isalpha(SourceCode[i])) {
                t += SourceCode[i];
                i++;
            }
            tokens.push_back(initialToken(t));
            t.clear();
            i--;
        }

        // --- Numbers ---
        else if (isdigit(SourceCode[i])) {
            while (i < SourceCode.size() && isdigit(SourceCode[i])) {
                t += SourceCode[i];
                i++;
            }
            tokens.push_back(initialToken(t));
            t.clear();
            i--;
        }

        // --- Symbols and Punctuations ---
        else if (ispunct(SourceCode[i])) {

            // Single-line comment starting with #
            if (SourceCode[i] == '#') {
                while (i < SourceCode.size() && SourceCode[i] != '\n') {
                    i++;
                }
            }

            // Multi-line comment /* */
            else if (SourceCode[i] == '/' && i + 1 < SourceCode.size() && SourceCode[i + 1] == '*') {
                i += 2; // Skip /*
                while (i + 1 < SourceCode.size() && !(SourceCode[i] == '*' && SourceCode[i + 1] == '/')) {
                    i++;
                }
                i += 2; // Skip */
            }

            // Strings between single quotes ' or double quotes "
            else if (isquote(SourceCode[i])) {
                char quote = SourceCode[i];
                t += SourceCode[i++]; // Add opening quote

                while (i < SourceCode.size() && SourceCode[i] != quote) {
                    t += SourceCode[i];
                    i++;
                }

                // Check if we found the closing quote
                if (i < SourceCode.size()) {
                    t += SourceCode[i]; // Add closing quote
                    tokens.push_back(initialToken(t));
                    t.clear();
                } else {
                    Ferror("Error: Unterminated string literal");
                }
            }

            // Brackets
            else if (isbrackets(SourceCode[i])) {
                t += SourceCode[i];
                tokens.push_back(initialToken(t));
                t.clear();
            }

            // Arithmetic operators and symbols
            else if (isoperator(SourceCode[i])) {
                t += SourceCode[i];
                tokens.push_back(initialToken(t));
                t.clear();
            } else if ( SourceCode[i] == ';') {
                t += SourceCode[i];
                tokens.push_back(initialToken(t));
                t.clear();
            }

        }
    }

    tokens.push_back(Token(Tokentype::END_OF_FILE, "END OF FILE"));
    return tokens;
}

Token Lexer::initialToken(const std::string& word) {
    Token t;
    t.Value = word;

    // 1. Reserved Keywords
    if (word == "print") {
        t.type = tokentype::PRINT;
    } else if (word == "let") {
        t.type = tokentype::LET;
    }

    // 2. Parentheses
    else if (word == "(") {
        t.type = tokentype::LEFT_PAREN;
    } else if (word == ")") {
        t.type = tokentype::RIGHT_PAREN;
    }

    // 3. Square Brackets
    else if (word == "[") {
        t.type = tokentype::LEFT_BRACKET;
    } else if (word == "]") {
        t.type = tokentype::RIGHT_BRACKET;
    }

    // 4. Curly Braces
    else if (word == "{") {
        t.type = tokentype::LEFT_BRACE;
    } else if (word == "}") {
        t.type = tokentype::RIGHT_BRACE;
    }

    // 5. Character Literal (between single quotes)
    else if (word.length() >= 2 && (word.front() == '\'' || word.back() == '\'')) {
        t.type = tokentype::CHAR_LITERAL;
    }

    // 6. String Literal (between double quotes)
    else if (word.length() >= 2 && (word.front() == '\"' || word.back() == '\"')) {
        t.type = tokentype::STRING;
    }

    // 7. Operators
    else if (word == "+" || word == "-" || word == "*" || word == "/" ||
             word == "=") {
        t.type = tokentype::OPERATOR;
    }

    // 8. Numbers
    else if (isNumber(word)) {
        t.type = tokentype::NUMBER;
    }
    else if (word == ";") {
        t.type = tokentype::SEMICOLON;
    }

    else if (word == "true" || word == "false") {
        t.type = tokentype::BOOL;
    }

    // 9. Identifier
    else {
        t.type = tokentype::IDENTIFIER;
    }

    return t;
}

Token& Lexer::operator[](int i) {
    return tokens[i];
}

int Lexer::size() const {
    return tokens.size();
}