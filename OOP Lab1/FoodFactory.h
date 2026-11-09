#pragma once
#include "ProductFactory.h"
#include "FoodProduct.h"
#include <memory>
#include <string>

class FoodFactory : public ProductFactory {
public:
    std::shared_ptr<Product> create_product(const std::string& id, const std::string& name,
        const std::string& manufacturer, // <--- Добавлен производитель
        int price, int quantity, double weight,
        const Date& production_date, int shelf_life_days) const;
};