#include <fstream>
#include <string>
#include <utility>
#include <vector>

#include <toml.hpp>

#include "type/lua_bind.hpp"

std::string generateOverloadFunction(std::pair<std::string, std::string> &operation,
  std::vector<std::pair<std::string, std::string>> &operatorVarCombination)
{
  std::string overloadFunction = "auto " + operation.second + "_overloads = sol::overload(\n";
  for (size_t i = 0; i < operatorVarCombination.size(); ++i) {
    overloadFunction += "    [](const " + operatorVarCombination[i].first + " &v1, " + operatorVarCombination[i].second
                        + " v2) { return v1 " + operation.first + " v2; }";
    if (i < operatorVarCombination.size() - 1) overloadFunction += ",\n";
  }
  overloadFunction += "\n);\n";
  return overloadFunction;
}

std::string generateConstructorParams(std::string &type, std::vector<std::string> &constructorParams)
{
  std::string paramsStr;
  for (size_t i = 0; i < constructorParams.size(); ++i) {
    paramsStr += type + "(" + constructorParams[i] + ")";
    if (i < constructorParams.size() - 1) paramsStr += ", ";
  }
  return paramsStr;
}

void generateBinding(BindingConfig &config, std::ofstream &headerFile, std::ofstream &implFile)
{
  std::vector<std::pair<std::string, std::string>> operatorVarCombination;

  auto typePair = std::make_pair(config.type, config.type);
  operatorVarCombination.emplace_back(typePair);

  for (auto var : config.operatorVar) {
    auto pair1 = std::make_pair(config.type, var);
    auto pair2 = std::make_pair(var, config.type);

    operatorVarCombination.emplace_back(pair1);
    operatorVarCombination.emplace_back(pair2);
  }

  headerFile << "// This file generated by AnterleTool.\n";
  headerFile << "// Dont modifiy it.\n";
  headerFile << "// clang-format off\n";

  headerFile << "#pragma once\n";
  headerFile << "#include <" << config.typeImport << ">\n";
  headerFile << "#include <sol/sol.hpp>\n\n";
  headerFile << "namespace Anterle::Scripting {\n";
  headerFile << "void bind_" << config.type.substr(config.type.find_last_of(':') + 1) << "(sol::state &lua);\n";
  headerFile << "}// namespace Anterle::Scripting\n";
  headerFile << "// clang-format on\n";

  implFile << "// This file generated by AnterleTool.\n";
  implFile << "// Dont modifiy it.\n";
  implFile << "// clang-format off\n";

  implFile << "namespace Anterle::Scripting {\n";
  implFile << "void bind_" << config.type.substr(config.type.find_last_of(':') + 1) << "(sol::state &lua)\n";
  implFile << "{\n";

  std::vector<std::pair<std::string, std::string>> operations = {
    { "*", "multiplication" }, { "/", "division" }, { "+", "addition" }, { "-", "subtraction" }
  };
  for (auto &operation : operations) { implFile << generateOverloadFunction(operation, operatorVarCombination); }

  implFile << "    lua.new_usertype<" << config.type << ">(\"" << config.type.substr(config.type.find_last_of(':') + 1)
           << "\",\n";
  implFile << "        sol::call_constructor,\n";
  implFile << "        sol::constructors<" << generateConstructorParams(config.type, config.constructorParams)
           << ">(),\n";

  for (size_t i = 0; i < config.bindVar.size(); ++i) {
    implFile << "        \"" << config.bindVar[i] << "\", &" << config.type << "::" << config.bindVar[i];
    if (i < config.bindVar.size() - 1) implFile << ",\n";
  }

  implFile << ",\n";
  implFile << "        sol::meta_function::multiplication, multiplication_overloads,\n";
  implFile << "        sol::meta_function::division, division_overloads,\n";
  implFile << "        sol::meta_function::addition, addition_overloads,\n";
  implFile << "        sol::meta_function::subtraction, subtraction_overloads,\n";

  for (size_t i = 0; i < config.bindFunction.size(); ++i) {
    implFile << "        \"" << config.bindFunction[i].first << "\", ";
    implFile << "[](const " << config.type << " &v) { return " << config.bindFunction[i].second << "(v); }";
    if (i < config.bindFunction.size() - 1) implFile << ",\n";
  }

  implFile << "\n    );\n";
  implFile << "}\n";
  implFile << "}// namespace Anterle::Scripting\n";
  implFile << "// clang-format on\n";
}

void proccessBindingConfig(std::string &sourcePath, std::string &destPath)
{
  BindingConfig config;
  auto data = toml::parse(sourcePath);
  config.typeImport = toml::get<std::string>(data["typeImport"]);
  config.type = toml::get<std::string>(data["type"]);

  auto constructorParamsArray = toml::get<std::vector<std::string>>(data["constructorParams"]);
  config.constructorParams = std::move(constructorParamsArray);

  auto operatorVarArray = toml::get<std::vector<std::string>>(data["operatorVar"]);
  config.operatorVar = std::move(operatorVarArray);

  auto bindVarArray = toml::get<std::vector<std::string>>(data["bindVar"]);
  config.bindVar = std::move(bindVarArray);

  auto bindFunctionArray = toml::get<std::vector<std::pair<std::string, std::string>>>(data["bindFunction"]);
  config.bindFunction = std::move(bindFunctionArray);

  std::ofstream destImpl(destPath + ".cpp");
  std::ofstream destHeader(destPath + ".hpp");

  generateBinding(config, destHeader, destImpl);

  destHeader.close();
  destImpl.close();
}
