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
};


int main() {
    // Установка UTF-8 кодировки для консоли Windows
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    
    // Для русского языка
    setlocale(LC_ALL, ".UTF8");
    
    menumanager manager;

    manager.readfromfile("menu.txt");

    cout << "=== отсортировано по времени ===\n";
    manager.sortbycookingtime();
    manager.printall();

    
    int t;
    cin >> t;
    cout << "\n=== фильтр по времени (" << t << ")===\n";
    manager.filterbytime(t);
    return 0;
} 