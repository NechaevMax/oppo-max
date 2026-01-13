#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <windows.h>
#include <stdexcept>
#include <iomanip>

using namespace std;

// ========== СИСТЕМНЫЕ ИСКЛЮЧЕНИЯ ==========
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

// ========== КЛАСС MenuItem ==========
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
        cout << "  Название: " << name_
            << " | Цена: " << fixed << setprecision(2) << price_
            << " | Время: " << cooking_time_ << " мин" << endl;
    }
};

// ========== ВСТРОЕННЫЕ ТЕСТЫ ==========
class TestRunner {
private:
    int passed_ = 0;
    int failed_ = 0;

public:
    template<typename Func>
    void test(const string& name, Func func) {
        try {
            func();
            cout << "[PASS] " << name << endl;
            passed_++;
        } catch (const exception& e) {
            cout << "[FAIL] " << name << " - " << e.what() << endl;
            failed_++;
        }
    }

    void printResults() {
        cout << "\n===== РЕЗУЛЬТАТЫ ТЕСТОВ =====" << endl;
        cout << "Пройдено: " << passed_ << endl;
        cout << "Не пройдено: " << failed_ << endl;
        cout << "Всего: " << (passed_ + failed_) << endl;
        cout << "===============================\n" << endl;
    }

    int getFailedCount() const { return failed_; }
};

void runTests() {
    TestRunner runner;

    runner.test("MenuItem: создание валидного элемента", []() {
        MenuItem m("Борщ", 5.0, 30);
        if (m.getName() != "Борщ" || m.getPrice() != 5.0 || m.getCookingTime() != 30) {
            throw runtime_error("Значения не совпадают");
        }
    });

    runner.test("MenuItem: пустое имя вызывает ValidationException", []() {
        try {
            MenuItem m("", 5.0, 30);
            throw runtime_error("Исключение не было выброшено");
        } catch (const ValidationException&) {}
    });

    runner.test("MenuItem: отрицательная цена вызывает ValidationException", []() {
        try {
            MenuItem m("Борщ", -5.0, 30);
            throw runtime_error("Исключение не было выброшено");
        } catch (const ValidationException&) {}
    });

    runner.test("MenuItem: отрицательное время вызывает ValidationException", []() {
        try {
            MenuItem m("Борщ", 5.0, -30);
            throw runtime_error("Исключение не было выброшено");
        } catch (const ValidationException&) {}
    });

    runner.test("MenuItem: setName с пустой строкой вызывает исключение", []() {
        MenuItem m("Борщ", 5.0, 30);
        try {
            m.setName("");
            throw runtime_error("Исключение не было выброшено");
        } catch (const ValidationException&) {}
    });

    runner.test("MenuItem: setPrice с отрицательным значением вызывает исключение", []() {
        MenuItem m("Борщ", 5.0, 30);
        try {
            m.setPrice(-5.0);
            throw runtime_error("Исключение не было выброшено");
        } catch (const ValidationException&) {}
    });

    runner.test("MenuItem: setCookingTime с отрицательным значением", []() {
        MenuItem m("Борщ", 5.0, 30);
        try {
            m.setCookingTime(-30);
            throw runtime_error("Исключение не было выброшено");
        } catch (const ValidationException&) {}
    });

    runner.test("MenuItem: получение значений", []() {
        MenuItem m("Блюдо", 10.5, 45);
        if (m.getName() != "Блюдо" || m.getPrice() != 10.5 || m.getCookingTime() != 45) {
            throw runtime_error("Значения не совпадают");
        }
    });

    runner.test("MenuItem: изменение имени", []() {
        MenuItem m("Старое", 5.0, 10);
        m.setName("Новое");
        if (m.getName() != "Новое") {
            throw runtime_error("Имя не изменилось");
        }
    });

    runner.test("MenuItem: изменение цены", []() {
        MenuItem m("Борщ", 5.0, 10);
        m.setPrice(7.5);
        if (m.getPrice() != 7.5) {
            throw runtime_error("Цена не изменилась");
        }
    });

    runner.test("MenuItem: изменение времени готовки", []() {
        MenuItem m("Борщ", 5.0, 30);
        m.setCookingTime(20);
        if (m.getCookingTime() != 20) {
            throw runtime_error("Время не изменилось");
        }
    });

    runner.printResults();
}

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    setlocale(LC_ALL, ".UTF8");

    cout << "╔════════════════════════════════════════════╗" << endl;
    cout << "║  ПРОГРАММА УПРАВЛЕНИЯ МЕНЮ РЕСТОРАНА      ║" << endl;
    cout << "║  С ВСТРОЕННЫМ МОДУЛЬНЫМ ТЕСТИРОВАНИЕМ     ║" << endl;
    cout << "╚════════════════════════════════════════════╝\n" << endl;

    runTests();

    MenuItem item1("Борщ", 5.0, 30);
    MenuItem item2("Салат Цезаря", 7.0, 10);
    MenuItem item3("Стейк", 12.0, 25);
    MenuItem item4("Пельмени", 4.0, 15);

    cout << "\n===== ПРИМЕРЫ РАБОТЫ =====" << endl;
    cout << "1. Все блюда:" << endl;
    item1.print();
    item2.print();
    item3.print();
    item4.print();

    cout << "\n2. Сортировка по времени:" << endl;
    vector<MenuItem> items = {item1, item2, item3, item4};
    sort(items.begin(), items.end(),
        [](const MenuItem& a, const MenuItem& b) {
            return a.getCookingTime() < b.getCookingTime();
        });
    
    for (const auto& item : items) {
        item.print();
    }

    cout << "\nПрограмма завершена успешно!" << endl;
    return 0;
}
