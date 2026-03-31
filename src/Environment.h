#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <map>
#include <string>
#include <variant>

// Required modification: Added std:: prefix for portability
using RunTimeValue = std::variant<double, std::string, bool, std::nullptr_t>;

class Environment {
  std::map<std::string, RunTimeValue> variables;
  Environment * parent;
public:
  Environment(Environment *pe):parent(pe){}
  Environment():parent(nullptr){}
  void define(const std::string &name, RunTimeValue v);
  RunTimeValue get(const std::string &name);
  bool exists(const std::string &name);
};

#endif