#include "menu/MenuManager.h"

#include <algorithm>

#include "menu/Exceptions.h"

namespace menu {

void MenuManager::sortByCookingTime() {
    std::sort(items_.begin(), items_.end(), [](const MenuItem& a, const MenuItem& b) {
        return a.cookingTimeMin() < b.cookingTimeMin();
    });
}

std::vector<MenuItem> MenuManager::filterByMaxCookingTime(int max_minutes) const {
    if (max_minutes < 0) {
        throw ValidationException("max_minutes не может быть отрицательным");
    }

    std::vector<MenuItem> result;
    result.reserve(items_.size());
    for (const auto& item : items_) {
        if (item.cookingTimeMin() <= max_minutes) {
            result.push_back(item);
        }
    }
    return result;
}

} // namespace menu
