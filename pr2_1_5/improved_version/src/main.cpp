#include <iostream>
#include <limits>

#include "menu/MenuManager.h"
#include "menu/MenuRepository.h"
#include "menu/Exceptions.h"

#ifdef _WIN32
#include <windows.h>
#endif

namespace {

void setUtf8ConsoleIfWindows() {
#ifdef _WIN32
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
#endif
}

int readIntSafely(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        int value;
        std::cin >> value;
        if (!std::cin.fail()) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Ошибка ввода. Повторите.\n";
    }
}

double readDoubleSafely(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        double value;
        std::cin >> value;
        if (!std::cin.fail()) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Ошибка ввода. Повторите.\n";
    }
}

std::string readLineSafely(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    return line;
}

void printItem(const menu::MenuItem& item) {
    std::cout << "Название: " << item.name()
              << ", Цена: " << item.price()
              << ", Время: " << item.cookingTimeMin() << " мин\n";
}

void printItems(const std::vector<menu::MenuItem>& items) {
    for (const auto& it : items) {
        printItem(it);
    }
}

} // namespace

int main() {
    setUtf8ConsoleIfWindows();

    try {
        menu::FileMenuRepository repo;
        menu::MenuManager manager;
        manager.setItems(repo.loadFromFile("menu.txt"));

        while (true) {
            std::cout << "\n===== МЕНЮ =====\n"
                      << "1. Показать все блюда\n"
                      << "2. Сортировать по времени приготовления\n"
                      << "3. Фильтр по времени приготовления\n"
                      << "4. Добавить блюдо\n"
                      << "5. Выход\n";

            const int choice = readIntSafely("Выбор: ");

            switch (choice) {
                case 1:
                    printItems(manager.items());
                    break;
                case 2:
                    manager.sortByCookingTime();
                    printItems(manager.items());
                    break;
                case 3: {
                    const int max_t = readIntSafely("Максимальное время (мин): ");
                    const auto filtered = manager.filterByMaxCookingTime(max_t);
                    if (filtered.empty()) {
                        std::cout << "Ничего не найдено.\n";
                    } else {
                        printItems(filtered);
                    }
                    break;
                }
                case 4: {
                    std::string name;
                    // после readIntSafely в буфере \n уже очищен
                    name = readLineSafely("Название: ");
                    const double price = readDoubleSafely("Цена: ");
                    const int t = readIntSafely("Время приготовления (мин): ");
                    manager.addItem(menu::MenuItem(name, price, t));
                    std::cout << "Добавлено!\n";
                    break;
                }
                case 5:
                    std::cout << "До свидания!\n";
                    return 0;
                default:
                    std::cout << "Неверный пункт меню.\n";
                    break;
            }
        }
    } catch (const menu::MenuException& e) {
        std::cerr << "Ошибка: " << e.what() << '\n';
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Критическая ошибка: " << e.what() << '\n';
        return 1;
    }
}
