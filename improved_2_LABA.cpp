#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <windows.h>
#include <stdexcept>
#include <optional>

using namespace std;

// Пользовательские исключения
class MenuException : public runtime_error {
public:
    explicit MenuException(const string& message) : runtime_error(message) {}
};

class FileException : public MenuException {
public:
    explicit FileException(const string& filename) 
        : MenuException("Ошибка открытия файла: " + filename) {}
};

class ParseException : public MenuException {
public:
    explicit ParseException(const string& line) 
        : MenuException("Ошибка парсинга строки: " + line) {}
};

class ValidationException : public MenuException {
public:
    explicit ValidationException(const string& field) 
        : MenuException("Ошибка валидации поля: " + field) {}
};

// Класс MenuItem - элемент меню
class MenuItem {
private:
    string name_;
    double price_;
    int cooking_time_;

public:
    MenuItem() : price_(0), cooking_time_(0) {}
    
    MenuItem(const string& name, double price, int cooking_time)
        : name_(name), price_(price), cooking_time_(cooking_time) {
        validate();
    }

    void validate() const {
        if (name_.empty()) {
            throw ValidationException("name - название не может быть пустым");
        }
        if (price_ < 0) {
            throw ValidationException("price - цена не может быть отрицательной");
        }
        if (cooking_time_ < 0) {
            throw ValidationException("cooking_time - время не может быть отрицательным");
        }
    }

    const string& getName() const { return name_; }
    double getPrice() const { return price_; }
    int getCookingTime() const { return cooking_time_; }

    void setName(const string& n) {
        if (n.empty()) {
            throw ValidationException("name - название не может быть пустым");
        }
        name_ = n;
    }

    void setPrice(double p) {
        if (p < 0) {
            throw ValidationException("price - цена не может быть отрицательной");
        }
        price_ = p;
    }

    void setCookingTime(int t) {
        if (t < 0) {
            throw ValidationException("cooking_time - время не может быть отрицательным");
        }
        cooking_time_ = t;
    }

    void print() const {
        cout << "название: " << name_
            << ", цена: " << price_
            << ", время: " << cooking_time_ << " мин" << endl;
    }
};

// Класс для работы с файлами
class MenuFileReader {
public:
    static MenuItem parseMenuItem(const string& line) {
        stringstream ss(line);
        string token, type;
        
        ss >> type;
        if (type != "menu") {
            throw ParseException(line);
        }

        string name;
        double price = -1;
        int cooking_time = -1;

        while (ss >> token) {
            size_t pos = token.find('=');
            if (pos == string::npos) continue;

            string key = token.substr(0, pos);
            string value = token.substr(pos + 1);

            if (value.front() == '"' && value.back() != '"') {
                string rest;
                while (ss >> rest) {
                    value += " " + rest;
                    if (rest.back() == '"') break;
                }
            }

            value = removeQuotes(value);

            if (key == "name") {
                name = value;
            } else if (key == "price") {
                try {
                    price = stod(value);
                } catch (const invalid_argument&) {
                    throw ParseException("Неверный формат цены: " + value);
                }
            } else if (key == "time") {
                try {
                    cooking_time = stoi(value);
                } catch (const invalid_argument&) {
                    throw ParseException("Неверный формат времени: " + value);
                }
            }
        }

        if (price < 0 || cooking_time < 0) {
            throw ParseException("Не все обязательные поля заполнены в строке: " + line);
        }

        return MenuItem(name, price, cooking_time);
    }

private:
    static string removeQuotes(const string& s) {
        if (s.size() >= 2 && s.front() == '"' && s.back() == '"') {
            return s.substr(1, s.size() - 2);
        }
        return s;
    }
};

// Класс MenuManager - управление меню
class MenuManager {
private:
    vector<MenuItem> items_;

public:
    void readFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            throw FileException(filename);
        }

        string line;
        int line_number = 0;
        while (getline(file, line)) {
            line_number++;
            if (line.empty()) continue;

            try {
                MenuItem item = MenuFileReader::parseMenuItem(line);
                items_.push_back(item);
            } catch (const MenuException& e) {
                cerr << "Ошибка на строке " << line_number << ": " << e.what() << endl;
            }
        }

        file.close();
        
        if (items_.empty()) {
            throw MenuException("Не удалось загрузить ни одного элемента меню");
        }
    }

    void sortByCookingTime() {
        sort(items_.begin(), items_.end(),
            [](const MenuItem& a, const MenuItem& b) {
                return a.getCookingTime() < b.getCookingTime();
            });
    }

    void filterByTime(int max_time) const {
        if (max_time < 0) {
            throw ValidationException("max_time - время не может быть отрицательным");
        }
        
        bool found = false;
        for (const auto& item : items_) {
            if (item.getCookingTime() <= max_time) {
                item.print();
                found = true;
            }
        }
        
        if (!found) {
            cout << "Блюд с временем приготовления <= " << max_time << " мин не найдено\n";
        }
    }

    void printAll() const {
        for (const auto& item : items_) {
            item.print();
        }
    }

    void addItemManually() {
        string name;
        double price;
        int cooking_time;

        cout << "\n=== ДОБАВИТЬ НОВОЕ БЛЮДО ===\n";
        
        cin.clear();
        cin.ignore(10000, '\n');
        
        cout << "Введите название блюда: ";
        cout.flush();
        getline(cin, name);

        if (name.empty()) {
            throw ValidationException("name - название не может быть пустым");
        }

        cout << "Введите цену: ";
        cout.flush();
        try {
            cin >> price;
            cin.ignore(10000, '\n');
            if (cin.fail()) {
                throw ValidationException("price - неверный формат цены");
            }
        } catch (...) {
            throw ValidationException("price - ошибка при вводе цены");
        }

        cout << "Введите время приготовления (мин): ";
        cout.flush();
        try {
            cin >> cooking_time;
            cin.ignore(10000, '\n');
            if (cin.fail()) {
                throw ValidationException("cooking_time - неверный формат времени");
            }
        } catch (...) {
            throw ValidationException("cooking_time - ошибка при вводе времени");
        }

        MenuItem new_item(name, price, cooking_time);
        items_.push_back(new_item);
        cout << "Блюдо '" << name << "' успешно добавлено!\n";
    }

    void displayMenu() {
        int choice;
        while (true) {
            cout << "\n========== МЕНЮ ==========\n";
            cout << "1. Вывести все блюда\n";
            cout << "2. Вывести отсортировано по времени\n";
            cout << "3. Фильтр по времени приготовления\n";
            cout << "4. Добавить новое блюдо\n";
            cout << "5. Выход\n";
            cout << "Выберите опцию: ";
            cout.flush();
            
            cin >> choice;
            cin.ignore(10000, '\n');
            
            if (cin.fail()) {
                cin.clear();
                cout << "Ошибка ввода. Попробуйте снова.\n";
                continue;
            }

            try {
                switch (choice) {
                    case 1:
                        cout << "\n=== ВСЕ БЛЮДА ===\n";
                        printAll();
                        break;
                    case 2:
                        cout << "\n=== ОТСОРТИРОВАНО ПО ВРЕМЕНИ ===\n";
                        sortByCookingTime();
                        printAll();
                        break;
                    case 3: {
                        int t;
                        cout << "Введите максимальное время (мин): ";
                        cin >> t;
                        cin.ignore(10000, '\n');
                        cout << "\n=== ФИЛЬТР ПО ВРЕМЕНИ (" << t << " мин) ===\n";
                        filterByTime(t);
                        break;
                    }
                    case 4:
                        addItemManually();
                        break;
                    case 5:
                        cout << "До свидания!\n";
                        return;
                    default:
                        cout << "Неправильный выбор. Попробуйте снова.\n";
                }
            } catch (const ValidationException& e) {
                cout << "Ошибка валидации: " << e.what() << endl;
            } catch (const MenuException& e) {
                cout << "Ошибка: " << e.what() << endl;
            } catch (const exception& e) {
                cout << "Неожиданная ошибка: " << e.what() << endl;
            }
        }
    }
};

int main() {
    // Установка UTF-8 кодировки для консоли Windows
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    
    // Для русского языка
    setlocale(LC_ALL, ".UTF8");
    
    try {
        MenuManager manager;

        cout << "Загружаю меню из файла menu.txt...\n";
        manager.readFromFile("menu.txt");

        manager.displayMenu();
    } catch (const FileException& e) {
        cerr << "Ошибка файла: " << e.what() << endl;
        return 1;
    } catch (const MenuException& e) {
        cerr << "Ошибка: " << e.what() << endl;
        return 1;
    } catch (const exception& e) {
        cerr << "Критическая ошибка: " << e.what() << endl;
        return 1;
    }

    return 0;
}
