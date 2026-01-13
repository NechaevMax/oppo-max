#include <gtest/gtest.h>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <windows.h>
#include <stdexcept>

using namespace std;

// ============= КОПИРУЕМ КЛАССЫ ИЗ ОСНОВНОЙ ПРОГРАММЫ =============

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
};

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

// ============= ТЕСТЫ =============

// ТЕСТЫ MenuItem
class MenuItemTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Этот метод вызывается перед каждым тестом
    }
};

TEST_F(MenuItemTest, ConstructorWithValidData) {
    MenuItem item("Борщ", 5.5, 30);
    EXPECT_EQ(item.getName(), "Борщ");
    EXPECT_DOUBLE_EQ(item.getPrice(), 5.5);
    EXPECT_EQ(item.getCookingTime(), 30);
}

TEST_F(MenuItemTest, ConstructorWithEmptyName) {
    EXPECT_THROW(MenuItem("", 5.5, 30), ValidationException);
}

TEST_F(MenuItemTest, ConstructorWithNegativePrice) {
    EXPECT_THROW(MenuItem("Борщ", -5.5, 30), ValidationException);
}

TEST_F(MenuItemTest, ConstructorWithNegativeCookingTime) {
    EXPECT_THROW(MenuItem("Борщ", 5.5, -30), ValidationException);
}

TEST_F(MenuItemTest, SetNameValid) {
    MenuItem item("Борщ", 5.5, 30);
    item.setName("Салат");
    EXPECT_EQ(item.getName(), "Салат");
}

TEST_F(MenuItemTest, SetNameEmpty) {
    MenuItem item("Борщ", 5.5, 30);
    EXPECT_THROW(item.setName(""), ValidationException);
}

TEST_F(MenuItemTest, SetPriceValid) {
    MenuItem item("Борщ", 5.5, 30);
    item.setPrice(7.0);
    EXPECT_DOUBLE_EQ(item.getPrice(), 7.0);
}

TEST_F(MenuItemTest, SetPriceNegative) {
    MenuItem item("Борщ", 5.5, 30);
    EXPECT_THROW(item.setPrice(-1.0), ValidationException);
}

TEST_F(MenuItemTest, SetPriceZero) {
    MenuItem item("Борщ", 5.5, 30);
    item.setPrice(0.0);
    EXPECT_DOUBLE_EQ(item.getPrice(), 0.0);
}

TEST_F(MenuItemTest, SetCookingTimeValid) {
    MenuItem item("Борщ", 5.5, 30);
    item.setCookingTime(45);
    EXPECT_EQ(item.getCookingTime(), 45);
}

TEST_F(MenuItemTest, SetCookingTimeNegative) {
    MenuItem item("Борщ", 5.5, 30);
    EXPECT_THROW(item.setCookingTime(-10), ValidationException);
}

TEST_F(MenuItemTest, SetCookingTimeZero) {
    MenuItem item("Борщ", 5.5, 30);
    item.setCookingTime(0);
    EXPECT_EQ(item.getCookingTime(), 0);
}

// ТЕСТЫ MenuFileReader::parseMenuItem
class MenuFileReaderTest : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(MenuFileReaderTest, ParseValidMenuItem) {
    string line = "menu name=\"Борщ\" price=5.5 time=30";
    MenuItem item = MenuFileReader::parseMenuItem(line);
    EXPECT_EQ(item.getName(), "Борщ");
    EXPECT_DOUBLE_EQ(item.getPrice(), 5.5);
    EXPECT_EQ(item.getCookingTime(), 30);
}

TEST_F(MenuFileReaderTest, ParseMenuItemWithSpaceInName) {
    string line = "menu name=\"Салат Цезарь\" price=7.0 time=10";
    MenuItem item = MenuFileReader::parseMenuItem(line);
    EXPECT_EQ(item.getName(), "Салат Цезарь");
    EXPECT_DOUBLE_EQ(item.getPrice(), 7.0);
    EXPECT_EQ(item.getCookingTime(), 10);
}

TEST_F(MenuFileReaderTest, ParseInvalidType) {
    string line = "dish name=\"Борщ\" price=5.5 time=30";
    EXPECT_THROW(MenuFileReader::parseMenuItem(line), ParseException);
}

TEST_F(MenuFileReaderTest, ParseMissingPrice) {
    string line = "menu name=\"Борщ\" time=30";
    EXPECT_THROW(MenuFileReader::parseMenuItem(line), ParseException);
}

TEST_F(MenuFileReaderTest, ParseMissingTime) {
    string line = "menu name=\"Борщ\" price=5.5";
    EXPECT_THROW(MenuFileReader::parseMenuItem(line), ParseException);
}

TEST_F(MenuFileReaderTest, ParseInvalidPrice) {
    string line = "menu name=\"Борщ\" price=invalid time=30";
    EXPECT_THROW(MenuFileReader::parseMenuItem(line), ParseException);
}

TEST_F(MenuFileReaderTest, ParseInvalidTime) {
    string line = "menu name=\"Борщ\" price=5.5 time=invalid";
    EXPECT_THROW(MenuFileReader::parseMenuItem(line), ParseException);
}

TEST_F(MenuFileReaderTest, ParseZeroPrice) {
    string line = "menu name=\"Борщ\" price=0 time=30";
    MenuItem item = MenuFileReader::parseMenuItem(line);
    EXPECT_DOUBLE_EQ(item.getPrice(), 0.0);
}

TEST_F(MenuFileReaderTest, ParseZeroTime) {
    string line = "menu name=\"Борщ\" price=5.5 time=0";
    MenuItem item = MenuFileReader::parseMenuItem(line);
    EXPECT_EQ(item.getCookingTime(), 0);
}

TEST_F(MenuFileReaderTest, ParseDoublePrice) {
    string line = "menu name=\"Борщ\" price=5.99 time=30";
    MenuItem item = MenuFileReader::parseMenuItem(line);
    EXPECT_DOUBLE_EQ(item.getPrice(), 5.99);
}

TEST_F(MenuFileReaderTest, ParseLargeCookingTime) {
    string line = "menu name=\"Борщ\" price=5.5 time=999";
    MenuItem item = MenuFileReader::parseMenuItem(line);
    EXPECT_EQ(item.getCookingTime(), 999);
}

// ТЕСТЫ валидации
class ValidationTest : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(ValidationTest, ValidateWithValidData) {
    MenuItem item("Стейк", 12.5, 25);
    EXPECT_NO_THROW(item.validate());
}

TEST_F(ValidationTest, ValidateEmptyName) {
    MenuItem item;
    item.setPrice(5.0);
    item.setCookingTime(30);
    EXPECT_THROW(item.validate(), ValidationException);
}

// ИНТЕГРАЦИОННЫЕ ТЕСТЫ
class IntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Создаем тестовый файл
        ofstream test_file("test_menu.txt");
        test_file << "menu name=\"Борщ\" price=5.5 time=30\n";
        test_file << "menu name=\"Салат Цезарь\" price=7.0 time=10\n";
        test_file << "menu name=\"Стейк\" price=12.5 time=25\n";
        test_file.close();
    }

    void TearDown() override {
        // Удаляем тестовый файл
        remove("test_menu.txt");
    }
};

TEST_F(IntegrationTest, ParseMultipleItems) {
    ifstream file("test_menu.txt");
    vector<MenuItem> items;
    string line;
    
    while (getline(file, line)) {
        if (!line.empty()) {
            MenuItem item = MenuFileReader::parseMenuItem(line);
            items.push_back(item);
        }
    }
    file.close();
    
    EXPECT_EQ(items.size(), 3);
    EXPECT_EQ(items[0].getName(), "Борщ");
    EXPECT_EQ(items[1].getName(), "Салат Цезарь");
    EXPECT_EQ(items[2].getName(), "Стейк");
}

TEST_F(IntegrationTest, SortByCookingTime) {
    ifstream file("test_menu.txt");
    vector<MenuItem> items;
    string line;
    
    while (getline(file, line)) {
        if (!line.empty()) {
            MenuItem item = MenuFileReader::parseMenuItem(line);
            items.push_back(item);
        }
    }
    file.close();
    
    sort(items.begin(), items.end(),
        [](const MenuItem& a, const MenuItem& b) {
            return a.getCookingTime() < b.getCookingTime();
        });
    
    EXPECT_EQ(items[0].getCookingTime(), 10);
    EXPECT_EQ(items[1].getCookingTime(), 25);
    EXPECT_EQ(items[2].getCookingTime(), 30);
}

// ГРАНИЧНЫЕ СЛУЧАИ
class EdgeCasesTest : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(EdgeCasesTest, VeryLargePrice) {
    MenuItem item("Борщ", 999999.99, 30);
    EXPECT_DOUBLE_EQ(item.getPrice(), 999999.99);
}

TEST_F(EdgeCasesTest, VeryLargeCookingTime) {
    MenuItem item("Борщ", 5.5, 999999);
    EXPECT_EQ(item.getCookingTime(), 999999);
}

TEST_F(EdgeCasesTest, LongDishName) {
    string long_name(1000, 'а');
    MenuItem item(long_name, 5.5, 30);
    EXPECT_EQ(item.getName(), long_name);
}

TEST_F(EdgeCasesTest, SpecialCharactersInName) {
    MenuItem item("Борщ-деревенский@2024!", 5.5, 30);
    EXPECT_EQ(item.getName(), "Борщ-деревенский@2024!");
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
