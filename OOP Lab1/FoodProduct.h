#pragma once
#include "PerishableProduct.h"

class FoodProduct : public PerishableProduct {
public:
    FoodProduct(const std::string& id, const std::string& name, const std::string& manufacturer,
        const Price& price, const Quantity& quantity, const Weight& weight,
        const Date& production_date, int shelf_life_days);

    std::unique_ptr<Product> clone() const override;
    void print(std::ostream& os) const override;
};