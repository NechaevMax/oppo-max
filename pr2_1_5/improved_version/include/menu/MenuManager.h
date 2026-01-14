#pragma once

#include <vector>

#include "MenuItem.h"

namespace menu {

class MenuManager {
public:
    void setItems(std::vector<MenuItem> items) { items_ = std::move(items); }

    const std::vector<MenuItem>& items() const noexcept { return items_; }

    void addItem(const MenuItem& item) { items_.push_back(item); }

    void sortByCookingTime();

    std::vector<MenuItem> filterByMaxCookingTime(int max_minutes) const;

private:
    std::vector<MenuItem> items_;
};

} // namespace menu
