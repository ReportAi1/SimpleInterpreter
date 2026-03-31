#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "Parser.h"
#include <vector>
class interpreter {
  std::vector<std::unique_ptr<ASTNode>> Parsing;
  Environment evn;

public:
  interpreter(std::vector<std::unique_ptr<ASTNode>> p) : Parsing(std::move(p)){}

  void run() {
    for (std::unique_ptr<ASTNode> & stmt : Parsing) {
      stmt->execute(evn);
    }
  }
};
#endif
