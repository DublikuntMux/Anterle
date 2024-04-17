#pragma once

#include <string>
#include <utility>
#include <vector>

struct BindingConfig
{
  std::string typeImport;
  std::string type;
  std::vector<std::string> constructorParams;
  std::vector<std::string> operatorVar;
  std::vector<std::string> bindVar;
  std::vector<std::pair<std::string, std::string>> bindFunction;
};

void proccessBindingConfig(std::string &sourcePath, std::string &destPath);
