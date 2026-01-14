#pragma once

#include <stdexcept>
#include <string>

namespace menu {

class MenuException : public std::runtime_error {
public:
    explicit MenuException(const std::string& msg) : std::runtime_error(msg) {}
};

class FileException final : public MenuException {
public:
    explicit FileException(const std::string& filename)
        : MenuException("Не удалось открыть файл: " + filename) {}
};

class ParseException final : public MenuException {
public:
    explicit ParseException(const std::string& details)
        : MenuException("Ошибка парсинга: " + details) {}
};

class ValidationException final : public MenuException {
public:
    explicit ValidationException(const std::string& details)
        : MenuException("Ошибка валидации: " + details) {}
};

} // namespace menu
