#pragma once
#include "ProductFactory.h"
#include "StandardProduct.h"
#include <memory>
#include <string>

class StandardFactory : public ProductFactory {
public:
    std::shared_ptr<Product> create_product(const std::string& id, const std::string& name,
        const std::string& manufacturer, // <--- Новое
        int price, int quantity, double weight) const;
};