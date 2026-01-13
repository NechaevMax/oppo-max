#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <windows.h>

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
        cout << "название: " << name
            << ", цена: " << price
            << ", время: " << cookingtime << " мин" << endl;
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
            cout << "ошибка открытия файла\n";
            return;
        }

        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;

            stringstream ss(line);
            string token;
            string type;

            ss >> type;

            if (type != "menu") continue;

            menuitem item;

            // читаем остальные поля любом порядке
            while (ss >> token) {
                size_t pos = token.find('=');
                if (pos == string::npos) continue;

                string key = token.substr(0, pos);
                string value = token.substr(pos + 1);

                // если значение в кавычках и с пробелами
                if (value.front() == '"' && value.back() != '"') {
                    string rest;
                    while (ss >> rest) {
                        value += " " + rest;
                        if (rest.back() == '"') break;
                    }
                }

                value = removequotes(value);

                if (key == "name")
                    item.setname(value);
                else if (key == "price")
                    item.setprice(stod(value));
                else if (key == "time")
                    item.setcookingtime(stoi(value));
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
        for (const auto& item : items) {
            if (item.getcookingtime() <= maxtime)
                item.print();
        }
    }

    void printall() const {
        for (const auto& item : items)
            item.print();
    }

    void displaymenu() {
        int choice;
        while (true) {
            cout << "\n========== ГЛАВНОЕ МЕНЮ РЕСТОРАНА ==========\n";
            cout << "1. Показать все блюда в меню\n";
            cout << "2. Показать блюда отсортированные по времени\n";
            cout << "3. Поиск блюд по времени приготовления\n";
            cout << "4. Выход из программы\n";
            cout << "Выберите пункт меню: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    cout << "\n========== ПОЛНОЕ МЕНЮ =========\n";
                    printall();
                    break;
                case 2:
                    cout << "\n========== МЕНЮ ОТСОРТИРОВАНО ПО ВРЕМЕНИ =========\n";
                    sortbycookingtime();
                    printall();
                    break;
                case 3: {
                    int t;
                    cout << "Введите максимальное время приготовления (мин): ";
                    cin >> t;
                    cout << "\n========== ПОИСК: ВРЕМЯ ДО " << t << " МИН =========\n";
                    filterbytime(t);
                    break;
                }
                case 4:
                    cout << "Спасибо, что пользуетесь нашим сервисом!\n";
                    return;
                default:
                    cout << "Ошибка: неправильный выбор. Пожалуйста, выберите корректный пункт.\n";
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
    
    menumanager manager;

    manager.readfromfile("menu.txt");

    manager.displaymenu();

    return 0;
} 