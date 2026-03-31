#define WIN32_LEAN_AND_MEAN
#include "Parser.h"
#include "../include/Tools.h"
#include "Token.h"

// --- Core Parser Functions ---

Parser::Parser(Lexer &l) : _lexer(l), pos(0) {}

Token Parser::currentToken() {
    if (pos >= _lexer.size()) return Token(Tokentype::END_OF_FILE, "");
    return _lexer[pos];
}

void Parser::advance() { pos++; }

bool Parser::match(Tokentype expected) {
    if (currentToken().type == expected) {
        advance();
        return true;
    }
    Ferror("Error: Expected token type " + std::to_string((int)expected) + " but found " + currentToken().Value);
    exit(1);
}

// --- Main Parsing Loop ---

std::vector<std::unique_ptr<ASTNode>> Parser::Parsering() {
    while(currentToken().type != Tokentype::END_OF_FILE) {
        if (currentToken().type == Tokentype::PRINT) {
            AstNodes.push_back(std::move(parserPrint()));
        } else if(currentToken().type == Tokentype::LET) {
            AstNodes.push_back(std::move(parserVariableAssginmet()));
        } else if (currentToken().type == Tokentype::IDENTIFIER) {
            AstNodes.push_back(std::move(parserVariableUpdate()));
        } else if (currentToken().type == Tokentype::SEMICOLON) {
            advance(); // Skip redundant semicolons
        } else {
            advance();
        }
    }
    return std::move(AstNodes);
}

// --- Statement Parsing ---

std::unique_ptr<ASTNode> Parser::parserPrint() {
    match(Tokentype::PRINT);
    match(Tokentype::LEFT_PAREN);

    std::vector<std::unique_ptr<ASTNode>> arguments;

    while (currentToken().type != Tokentype::RIGHT_PAREN && currentToken().type != Tokentype::END_OF_FILE) {
        // Use parseExpression to handle (strings, numbers, or equations)
        arguments.push_back(parseExpression());

        // Handle comma separators between values (optional depending on language design)
        if (currentToken().Value == ",") { advance(); }
    }

    match(Tokentype::RIGHT_PAREN);
    if (currentToken().type == Tokentype::SEMICOLON) advance();

    return std::make_unique<PrintNode>(std::move(arguments));
}

std::unique_ptr<ASTNode> Parser::parserVariableAssginmet() {
    match(Tokentype::LET);
    std::string name = currentToken().Value;
    match(Tokentype::IDENTIFIER);

    // Check if there is an assignment (e.g., let x = 5)
    if (currentToken().Value == "=") {
        match(Tokentype::OPERATOR);

        // Core logic: Call parseExpression to fetch the "equation" as a Node
        auto expr = parseExpression();

        if (currentToken().type == Tokentype::SEMICOLON) advance();

        // Pass the expr (Node) rather than the final evaluated value
        return std::make_unique<VariableAssginmet>(name, std::move(expr));
    }

    // In case of 'let x;' without assignment, default to NumberNode(0)
    if (currentToken().type == Tokentype::SEMICOLON) advance();
    return std::make_unique<VariableAssginmet>(name, std::make_unique<NumberNode>(0));
}

std::unique_ptr<ASTNode> Parser::parserVariableUpdate() {
    std::string varName = currentToken().Value;
    match(Tokentype::IDENTIFIER);

    if (currentToken().Value == "=") {
        match(Tokentype::OPERATOR);

        // Fetch the equation as a Node
        auto expr = parseExpression();

        if (currentToken().type == Tokentype::SEMICOLON) advance();

        // Pass the expr to the Update Node
        return std::make_unique<VariableUpdate>(varName, std::move(expr));
    }

    Ferror("Error: Expected '=' after variable name");
    return nullptr;
}

// --- Recursive Descent Expression Engine ---

// 1. Addition and Subtraction (Lowest Precedence)
std::unique_ptr<ASTNode> Parser::parseExpression() {
    auto left = parseTerm();

    while (currentToken().Value == "+" || currentToken().Value == "-") {
        std::string op = currentToken().Value;
        advance();
        auto right = parseTerm();
        left = std::make_unique<BinaryExpr>(std::move(left), op, std::move(right));
    }
    return left;
}

// 2. Multiplication and Division (Medium Precedence)
std::unique_ptr<ASTNode> Parser::parseTerm() {
    auto left = parsePrimary();

    while (currentToken().Value == "*" || currentToken().Value == "/") {
        std::string op = currentToken().Value;
        advance();
        auto right = parsePrimary();
        left = std::make_unique<BinaryExpr>(std::move(left), op, std::move(right));
    }
    return left;
}

// 3. Primary Values (Highest Precedence: Numbers, Parentheses, Variables)
std::unique_ptr<ASTNode> Parser::parsePrimary() {
    // Numbers
    if (currentToken().type == Tokentype::NUMBER) {
        double val = std::stod(currentToken().Value);
        advance();
        return std::make_unique<NumberNode>(val);
    }

    // Strings
    if (currentToken().type == Tokentype::STRING) {
        std::string val = removeisquote(currentToken().Value);
        advance();
        return std::make_unique<StringNode>(val);
    }

    // Variables
    if (currentToken().type == Tokentype::IDENTIFIER) {
        std::string name = currentToken().Value;
        advance();
        return std::make_unique<VariableAccess>(name);
    }

    // Parentheses ( )
    if (currentToken().type == Tokentype::LEFT_PAREN) {
        advance(); // Skip (
        auto expr = parseExpression(); // Recurse inside the parentheses
        match(Tokentype::RIGHT_PAREN); // Match )
        return expr;
    }

    Ferror("Error: Unexpected token in expression: " + currentToken().Value);
    return nullptr;
}