#ifndef PARSER_H
#define PARSER_H

#include "Lexer.h"
#include "Token.h"
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <variant>

#include "Environment.h"

// 1. Base Class - Interface for all AST nodes
struct ASTNode {
  virtual ~ASTNode() = default;
  virtual void execute(Environment & evn) = 0;
  [[nodiscard]] virtual std::string getType() const = 0;
  virtual RunTimeValue evaluate(Environment & evn) = 0;
};

// 2. Number Node
struct NumberNode : public ASTNode {
  double value;
  NumberNode(double v) : value(v) {}

  RunTimeValue evaluate(Environment &evn) override {
    return value;
  }

  void execute(Environment &evn) override {
    // Numbers alone do nothing during execution
  }

  [[nodiscard]] std::string getType() const override {
    return "Number";
  }
};

// 3. Binary Expression Node (Addition, Subtraction, Multiplication, Division)
struct BinaryExpr : public ASTNode {
  std::unique_ptr<ASTNode> left;
  std::string op;
  std::unique_ptr<ASTNode> right;

  BinaryExpr(std::unique_ptr<ASTNode> l, std::string op, std::unique_ptr<ASTNode> r)
    : left(std::move(l)), op(op), right(std::move(r)) {}

  void execute(Environment &evn) override {
    // Execution here is simply evaluating the value
    evaluate(evn);
  }

  // Corrected this line (removed = 0)
  [[nodiscard]] std::string getType() const override {
    return "BinaryExpr";
  }

  RunTimeValue evaluate(Environment & evn) override {
    auto lVal = this->left->evaluate(evn);
    auto rVal = this->right->evaluate(evn);

    // Calculation is for numbers only
    if (std::holds_alternative<double>(lVal) && std::holds_alternative<double>(rVal)) {
        double v1 = std::get<double>(lVal);
        double v2 = std::get<double>(rVal);

        if (op == "+") return v1 + v2;
        if (op == "-") return v1 - v2;
        if (op == "*") return v1 * v2;
        if (op == "/") return (v2 != 0) ? v1 / v2 : 0.0;
    }
    return 0.0;
  }
};

// 4. String Node
struct StringNode : public ASTNode {
  std::string value;
  StringNode(std::string v): value(std::move(v)) {}

  void execute(Environment & evn) override {
    // 1. Get the processed value (handling \t or \n)
    RunTimeValue val = evaluate(evn);

    // 2. Print it using std::visit since it's a variant
    std::visit([](auto&& arg) {
        std::cout << arg;
    }, val);
  }

  RunTimeValue evaluate(Environment &evn) override {
    // Handle escape sequences like \t and \n
    std::string processed = value;
    size_t pos = 0;

    // Convert literal \t to an actual Tab
    while ((pos = processed.find("\\t", pos)) != std::string::npos) {
      processed.replace(pos, 2, "\t");
      pos += 1;
    }

    // Convert literal \n to an actual newline
    pos = 0;
    while ((pos = processed.find("\\n", pos)) != std::string::npos) {
      processed.replace(pos, 2, "\n");
      pos += 1;
    }

    return processed;
  }

  std::string getType() const override { return "String"; }
};

// 5. Print Node
struct PrintNode : public ASTNode {
  std::vector<std::unique_ptr<ASTNode>> arguments;
  PrintNode(std::vector<std::unique_ptr<ASTNode>> && args): arguments(std::move(args)) {};

  void execute(Environment & evn) override {
    for (auto & argument : arguments) {
      if (argument) {
        // Use evaluate to calculate expression results before printing
        RunTimeValue val = argument->evaluate(evn);
        std::visit([](auto && arg) { std::cout << arg; }, val);
      }
    }
    std::cout << std::endl; // New line after each print command
  }

  std::string getType() const override {
    return "print";
  }

  RunTimeValue evaluate(Environment &evn) override {
    return nullptr;
  }
};

// 6. Variable Access Node (Reading its value)
struct VariableAccess : public ASTNode {
  std::string varName;
  VariableAccess(std::string n): varName(n){}

  void execute(Environment & evn) override {
    // Does nothing except fetching the value
  }

  RunTimeValue evaluate(Environment & evn) override {
    return evn.get(varName);
  }

  std::string getType() const override {
    return "VariableAccess";
  }
};

// 7. Variable Assignment Node (Defining a new variable)
struct VariableAssginmet : public ASTNode {
  std::string varName;
  std::unique_ptr<ASTNode> expression; // Now stores the AST tree (expression)

  VariableAssginmet(std::string vn, std::unique_ptr<ASTNode> exp)
      : varName(vn), expression(std::move(exp)) {}

  void execute(Environment &evn) override {
    // Evaluation is now performed using the actual environment
    evn.define(varName, expression->evaluate(evn));
  }

  RunTimeValue evaluate(Environment &evn) override {
    return expression->evaluate(evn);
  }

  std::string getType() const override { return "VariableAssginmet"; }
};

// 8. Variable Update Node (Updating an existing variable)
struct VariableUpdate : public ASTNode {
  std::string varName;
  std::unique_ptr<ASTNode> expression;

  VariableUpdate(std::string vn, std::unique_ptr<ASTNode> exp)
      : varName(vn), expression(std::move(exp)) {}

  void execute(Environment &evn) override {
    if (evn.exists(varName)) {
      evn.define(varName, expression->evaluate(evn));
    } else {
      std::cerr << "Runtime Error: Variable '" << varName << "' not defined." << std::endl;
    }
  }

  RunTimeValue evaluate(Environment &evn) override {
    return expression->evaluate(evn);
  }

  std::string getType() const override { return "VariableUpdate"; }
};

// 9. Variable Get Value Node (e.g., x = y)
struct VariableGetValue : public ASTNode {
  std::string varName;
  std::string varNex;

  VariableGetValue(std::string vn, std::string vN): varName(vn), varNex(vN) {}

  void execute(Environment &evn) override {
    if (evn.exists(varNex)) {
      evn.define(varName, evn.get(varNex));
    }
  }

  RunTimeValue evaluate(Environment &evn) override {
    return evn.get(varNex);
  }

  [[nodiscard]] std::string getType() const override {
    return "VariableGetValue";
  }
};

// 10. Parser Class
class Parser {
  std::vector<std::unique_ptr<ASTNode>> AstNodes;
private:
  Lexer &_lexer;
  int pos = 0;
public:
  Token currentToken();
  Parser(Lexer &l);
  bool match(Tokentype expected);
  void advance();
  std::vector<std::unique_ptr<ASTNode>> Parsering();
  std::unique_ptr<ASTNode> parserPrint();
  std::unique_ptr<ASTNode> parserVariableAssginmet();
  std::unique_ptr<ASTNode> parserVariableUpdate();

  // Mathematical expression functions
  std::unique_ptr<ASTNode> parsePrimary();
  std::unique_ptr<ASTNode> parseTerm();
  std::unique_ptr<ASTNode> parseExpression();
};

#endif