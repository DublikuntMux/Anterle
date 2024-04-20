#pragma once

#include <memory>
#include <string>

namespace Anterle::Utils {
std::pair<std::shared_ptr<char>, std::size_t> LoadRawDataFromFile(const std::string &filePath);

void SetupImGuiStyle();
}// namespace Anterle::Utils
