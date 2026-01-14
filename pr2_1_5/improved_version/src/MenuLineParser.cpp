#include "menu/MenuLineParser.h"

#include <cctype>
#include <sstream>

namespace menu {

std::string MenuLineParser::trim(const std::string& s) {
    size_t start = 0;
    while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) {
        ++start;
    }
    size_t end = s.size();
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1]))) {
        --end;
    }
    return s.substr(start, end - start);
}

std::string MenuLineParser::unquote(const std::string& s) {
    if (s.size() >= 2 && s.front() == '"' && s.back() == '"') {
        return s.substr(1, s.size() - 2);
    }
    return s;
}

std::unordered_map<std::string, std::string> MenuLineParser::parseKeyValues(const std::string& raw_line) {
    std::string line = trim(raw_line);

    // 1) Проверяем префикс "menu"
    {
        std::istringstream ss(line);
        std::string type;
        ss >> type;
        if (type != "menu") {
            throw ParseException("ожидался префикс 'menu': " + raw_line);
        }
    }

    // 2) Разбор пары key=value, где value может быть в кавычках и содержать пробелы
    std::unordered_map<std::string, std::string> kv;

    size_t i = 0;
    // пропускаем первое слово "menu"
    while (i < line.size() && !std::isspace(static_cast<unsigned char>(line[i]))) {
        ++i;
    }
    while (i < line.size()) {
        while (i < line.size() && std::isspace(static_cast<unsigned char>(line[i]))) {
            ++i;
        }
        if (i >= line.size()) break;

        // key
        size_t key_start = i;
        while (i < line.size() && line[i] != '=' && !std::isspace(static_cast<unsigned char>(line[i]))) {
            ++i;
        }
        if (i >= line.size() || line[i] != '=') {
            // мусорный токен
            while (i < line.size() && !std::isspace(static_cast<unsigned char>(line[i]))) {
                ++i;
            }
            continue;
        }
        std::string key = line.substr(key_start, i - key_start);
        ++i; // '='

        if (i >= line.size()) {
            throw ParseException("пустое значение для ключа '" + key + "': " + raw_line);
        }

        std::string value;
        if (line[i] == '"') {
            // значение в кавычках
            size_t val_start = i;
            ++i; // после первой кавычки
            while (i < line.size() && line[i] != '"') {
                ++i;
            }
            if (i >= line.size()) {
                throw ParseException("нет закрывающей кавычки для ключа '" + key + "': " + raw_line);
            }
            ++i; // включая закрывающую кавычку
            value = line.substr(val_start, i - val_start);
        } else {
            // значение до следующего пробела
            size_t val_start = i;
            while (i < line.size() && !std::isspace(static_cast<unsigned char>(line[i]))) {
                ++i;
            }
            value = line.substr(val_start, i - val_start);
        }

        kv[trim(key)] = trim(value);
    }

    return kv;
}

MenuItem MenuLineParser::parseLine(const std::string& line) {
    const auto kv = parseKeyValues(line);

    auto it_name = kv.find("name");
    auto it_price = kv.find("price");
    auto it_time = kv.find("time");

    if (it_name == kv.end()) {
        throw ParseException("не найдено поле name: " + line);
    }
    if (it_price == kv.end()) {
        throw ParseException("не найдено поле price: " + line);
    }
    if (it_time == kv.end()) {
        throw ParseException("не найдено поле time: " + line);
    }

    const std::string name = unquote(it_name->second);

    double price = 0.0;
    try {
        price = std::stod(unquote(it_price->second));
    } catch (...) {
        throw ParseException("некорректное число price: " + it_price->second);
    }

    int time_min = 0;
    try {
        time_min = std::stoi(unquote(it_time->second));
    } catch (...) {
        throw ParseException("некорректное число time: " + it_time->second);
    }

    return MenuItem(name, price, time_min);
}

} // namespace menu
