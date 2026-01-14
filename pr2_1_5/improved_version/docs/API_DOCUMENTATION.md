# API документация (автогенерация)

Источник: заголовочные файлы `include/menu/*.h`.

## FileException

_Публичные объявления методов не обнаружены._

## FileMenuRepository

Публичные методы/конструкторы:

- `std::vector<MenuItem> loadFromFile(const std::string& filename) const override;`

## MenuException

_Публичные объявления методов не обнаружены._

## MenuItem

Публичные методы/конструкторы:

- `MenuItem() = default;`

## MenuLineParser

Публичные методы/конструкторы:

- `static MenuItem parseLine(const std::string& line);`

## MenuManager

Публичные методы/конструкторы:

- `void sortByCookingTime();`
- `std::vector<MenuItem> filterByMaxCookingTime(int max_minutes) const;`

## MenuRepository

Публичные методы/конструкторы:

- `virtual ~MenuRepository() = default;`

## ParseException

_Публичные объявления методов не обнаружены._

## ValidationException

_Публичные объявления методов не обнаружены._
