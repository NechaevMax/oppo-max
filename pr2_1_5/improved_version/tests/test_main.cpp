#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "menu/MenuLineParser.h"
#include "menu/MenuManager.h"
#include "menu/MenuRepository.h"

namespace {

int g_failed = 0;

#define EXPECT_TRUE(cond) do { \
    if (!(cond)) { \
        std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ << " expected TRUE: " #cond << "\n"; \
        ++g_failed; \
    } \
} while(0)

#define EXPECT_EQ(a,b) do { \
    auto _a = (a); \
    auto _b = (b); \
    if (!(_a == _b)) { \
        std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ << " expected: " #a " == " #b << " (" << _a << " vs " << _b << ")\n"; \
        ++g_failed; \
    } \
} while(0)

#define EXPECT_THROW(stmt) do { \
    bool thrown = false; \
    try { (void)(stmt); } catch (...) { thrown = true; } \
    if (!thrown) { \
        std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ << " expected exception: " #stmt << "\n"; \
        ++g_failed; \
    } \
} while(0)

void testParseLine() {
    const std::string line = "menu name=\"Салат Цезарь\" price=7.0 time=10";
    const auto item = menu::MenuLineParser::parseLine(line);
    EXPECT_EQ(item.name(), std::string("Салат Цезарь"));
    EXPECT_EQ(item.price(), 7.0);
    EXPECT_EQ(item.cookingTimeMin(), 10);
}

void testParseInvalid() {
    EXPECT_THROW(menu::MenuLineParser::parseLine("bad name=\"x\" price=1 time=1"));
    EXPECT_THROW(menu::MenuLineParser::parseLine("menu name=\"x\" price=abc time=1"));
    EXPECT_THROW(menu::MenuLineParser::parseLine("menu name=\"x\" price=1 time=-1"));
}

void testManagerSortAndFilter() {
    menu::MenuManager m;
    m.setItems({
        menu::MenuItem("A", 1.0, 30),
        menu::MenuItem("B", 1.0, 10),
        menu::MenuItem("C", 1.0, 20),
    });
    m.sortByCookingTime();
    EXPECT_EQ(m.items().at(0).name(), std::string("B"));
    EXPECT_EQ(m.items().at(2).name(), std::string("A"));

    auto filtered = m.filterByMaxCookingTime(15);
    EXPECT_EQ(filtered.size(), static_cast<size_t>(1));
    EXPECT_EQ(filtered.at(0).name(), std::string("B"));
}

void testRepositoryLoad() {
    const std::string tmp = "tmp_menu_test.txt";
    {
        std::ofstream out(tmp);
        out << "menu name=\"Borsh\" price=5.5 time=30\n";
        out << "menu name=\"Bad\" price=abc time=1\n"; // будет проигнорировано
        out << "menu name=\"Salad\" price=2 time=5\n";
    }

    menu::FileMenuRepository repo;
    const auto items = repo.loadFromFile(tmp);
    EXPECT_EQ(items.size(), static_cast<size_t>(2));
    EXPECT_EQ(items.at(0).name(), std::string("Borsh"));
    EXPECT_EQ(items.at(1).name(), std::string("Salad"));

    std::remove(tmp.c_str());
}

} // namespace

int main() {
    std::cout << "Running tests...\n";

    testParseLine();
    testParseInvalid();
    testManagerSortAndFilter();
    testRepositoryLoad();

    if (g_failed == 0) {
        std::cout << "[OK] All tests passed\n";
        return 0;
    }
    std::cerr << "[FAILED] " << g_failed << " test(s)\n";
    return 1;
}
