#pragma once

#include <string>
#include <unordered_map>
#include "Exceptions.h"
#include "MenuItem.h"

namespace menu {

class MenuLineParser {
public:
    // Формат строки:
    // menu name="Салат Цезарь" price=7.0 time=10
    static MenuItem parseLine(const std::string& line);

private:
    static std::string trim(const std::string& s);
    static std::unordered_map<std::string, std::string> parseKeyValues(const std::string& line);
    static std::string unquote(const std::string& s);
};

} // namespace menu
