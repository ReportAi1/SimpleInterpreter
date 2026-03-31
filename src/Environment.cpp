#include "Environment.h"
#include <iostream>
#include "../include/Tools.h"

void Environment::define(const std::string &name, RunTimeValue v) {
  variables[name] = v;
}
RunTimeValue Environment::get(const std::string &name) {
  auto it = variables.find(name);
  if (it != variables.end()) {
    return it->second;
  }
  //ferror("variable " + std::to_string(name) + " not defined");
  return "Null";
}

bool Environment::exists(const std::string &name) {
  return variables.find(name) != variables.end();
}
