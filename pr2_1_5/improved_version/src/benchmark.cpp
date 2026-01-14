#include <iostream>
#include <vector>

#include "menu/MenuLineParser.h"
#include "menu/MenuManager.h"

int main() {
    const std::vector<std::string> lines = {
        "menu name=\"Борщ\" price=5.5 time=30",
        "menu name=\"Салат Цезарь\" price=7.0 time=10",
        "menu name=\"Стейк\" price=12.5 time=25",
        "menu name=\"Пельмени\" price=4.0 time=15",
    };

    menu::MenuManager manager;
    // наполняем начальными данными
    std::vector<menu::MenuItem> base;
    base.reserve(400);
    for (int i = 0; i < 100; ++i) {
        for (const auto& l : lines) {
            base.push_back(menu::MenuLineParser::parseLine(l));
        }
    }
    manager.setItems(base);

    // Нагрузка для профайлера: парсинг + сортировка + фильтрация
    for (int i = 0; i < 50000; ++i) {
        (void)menu::MenuLineParser::parseLine(lines[i % lines.size()]);
        manager.sortByCookingTime();
        (void)manager.filterByMaxCookingTime(i % 60);
    }

    std::cout << "benchmark done\n";
    return 0;
}
