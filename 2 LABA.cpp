#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <locale>
#include <windows.h>
#include <limits>


using namespace std;

class menuitem {
private:
    string name;
    double price;
    int cookingtime;

public:
    menuitem() : price(0), cookingtime(0) {}

    void setname(const string& n) {
        name = n;
    }

    void setprice(double p) {
        price = p;
    }

    void setcookingtime(int t) {
        cookingtime = t;
    }

    int getcookingtime() const {
        return cookingtime;
    }

    void print() const {
        cout << "Название: " << name
            << ", Цена: " << price
            << ", Время: " << cookingtime << " мин" << endl;
    }
};

class menumanager {
private:
    vector<menuitem> items;

    string removequotes(const string& s) {
        if (s.size() >= 2 && s.front() == '"' && s.back() == '"')
            return s.substr(1, s.size() - 2);
        return s;
    }

public:
    void readfromfile(const string& filename) {
        ifstream file(filename);
        if (!file) {
            cout << "Ошибка открытия файла\n";
            return;
        }

        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;

            stringstream ss(line);
            string token, type;
            ss >> type;

            if (type != "menu") continue;

            menuitem item;

            while (ss >> token) {
                size_t pos = token.find('=');
                if (pos == string::npos) continue;

                string key = token.substr(0, pos);
                string value = token.substr(pos + 1);

                if (!value.empty() && value.front() == '"' && value.back() != '"') {
                    string rest;
                    while (ss >> rest) {
                        value += " " + rest;
                        if (!rest.empty() && rest.back() == '"') break;
                    }
                }

                value = removequotes(value);

                if (key == "name")
                    item.setname(value);
                else if (key == "price")
                    item.setprice(atof(value.c_str()));
                else if (key == "time")
                    item.setcookingtime(atoi(value.c_str()));
            }

            items.push_back(item);
        }

        file.close();
    }

    void sortbycookingtime() {
        sort(items.begin(), items.end(),
            [](const menuitem& a, const menuitem& b) {
                return a.getcookingtime() < b.getcookingtime();
            });
    }

    void filterbytime(int maxtime) const {
        for (const auto& item : items)
            if (item.getcookingtime() <= maxtime)
                item.print();
    }

    void printall() const {
        for (const auto& item : items)
            item.print();
    }

    void additemmanually() {
        string name;
        double price;
        int cookingtime;

        cout << "\n=== ДОБАВИТЬ НОВОЕ БЛЮДО ===\n";

        // корректно очищаем поток
        cin.clear();
        cin.ignore(10000, '\n');

        cout << "Введите название: ";
        getline(cin, name);

        if (name.empty()) {
            cout << "Ошибка: название не может быть пустым\n";
            return;
        }

        cout << "Введите цену: ";
        cin >> price;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Ошибка: неверный формат цены\n";
            return;
        }

        cout << "Введите время приготовления (мин): ";
        cin >> cookingtime;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Ошибка: неверный формат времени\n";
            return;
        }

        menuitem item;
        item.setname(name);
        item.setprice(price);
        item.setcookingtime(cookingtime);

        items.push_back(item);
        cout << "Блюдо '" << name << "' успешно добавлено!\n";
    }

    void displaymenu() {
        int choice;
        while (true) {
            cout << "\n===== МЕНЮ =====\n";
            cout << "1. Показать все блюда\n";
            cout << "2. Сортировать по времени\n";
            cout << "3. Фильтр по времени\n";
            cout << "4. Добавить блюдо\n";
            cout << "5. Выход\n";
            cout << "Выбор: ";

            cin >> choice;

            switch (choice) {
            case 1:
                printall();
                break;
            case 2:
                sortbycookingtime();
                printall();
                break;
            case 3: {
                int t;
                cout << "Максимальное время: ";
                cin >> t;
                filterbytime(t);
                break;
            }
            case 4:
                additemmanually();
                break;
            case 5:
                cout << "До свидания!\n";
                return;
            default:
                cout << "Неверный пункт меню\n";
            }
        }
    }
};

int main() {
    // Устанавливаем кодировку консоли Windows на UTF-8
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    setlocale(LC_ALL, ".UTF8");
    
    // Для совместимости с Windows консолью
    ios_base::sync_with_stdio(false);
    cout.imbue(locale(""));

    menumanager manager;
    manager.readfromfile("menu.txt");
    manager.displaymenu();

    return 0;
}

