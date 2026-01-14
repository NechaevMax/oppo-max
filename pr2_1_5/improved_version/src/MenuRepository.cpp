#include "menu/MenuRepository.h"

#include <fstream>
#include <iostream>

#include "menu/MenuLineParser.h"
#include "menu/Exceptions.h"

namespace menu {

std::vector<MenuItem> FileMenuRepository::loadFromFile(const std::string& filename) const {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw FileException(filename);
    }

    std::vector<MenuItem> items;
    std::string line;
    std::size_t line_no = 0;

    while (std::getline(file, line)) {
        ++line_no;
        if (line.empty()) {
            continue;
        }
        try {
            items.push_back(MenuLineParser::parseLine(line));
        } catch (const MenuException& e) {
            // Для отчета важно фиксировать ошибки входных данных.
            std::cerr << "[WARN] строка " << line_no << ": " << e.what() << '\n';
        }
    }

    return items;
}

} // namespace menu
