#pragma once

#include <string>
#include <vector>

#include "MenuItem.h"

namespace menu {

class MenuRepository {
public:
    virtual ~MenuRepository() = default;
    virtual std::vector<MenuItem> loadFromFile(const std::string& filename) const = 0;
};

class FileMenuRepository final : public MenuRepository {
public:
    std::vector<MenuItem> loadFromFile(const std::string& filename) const override;
};

} // namespace menu
