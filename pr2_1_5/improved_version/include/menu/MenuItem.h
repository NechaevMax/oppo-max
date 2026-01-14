#pragma once

#include <string>
#include "Exceptions.h"

namespace menu {

class MenuItem {
public:
    MenuItem() = default;

    MenuItem(std::string name, double price, int cooking_time_min)
        : name_(std::move(name)), price_(price), cooking_time_min_(cooking_time_min) {
        validate();
    }

    const std::string& name() const noexcept { return name_; }
    double price() const noexcept { return price_; }
    int cookingTimeMin() const noexcept { return cooking_time_min_; }

    void setName(std::string name) {
        if (name.empty()) {
            throw ValidationException("поле name не может быть пустым");
        }
        name_ = std::move(name);
    }

    void setPrice(double price) {
        if (price < 0.0) {
            throw ValidationException("поле price не может быть отрицательным");
        }
        price_ = price;
    }

    void setCookingTimeMin(int minutes) {
        if (minutes < 0) {
            throw ValidationException("поле time не может быть отрицательным");
        }
        cooking_time_min_ = minutes;
    }

private:
    void validate() const {
        if (name_.empty()) {
            throw ValidationException("поле name не заполнено");
        }
        if (price_ < 0.0) {
            throw ValidationException("поле price имеет отрицательное значение");
        }
        if (cooking_time_min_ < 0) {
            throw ValidationException("поле time имеет отрицательное значение");
        }
    }

    std::string name_;
    double price_{0.0};
    int cooking_time_min_{0};
};

} // namespace menu
