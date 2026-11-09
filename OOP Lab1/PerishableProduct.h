#pragma once
#include "Product.h"
#include "Date.h"

class PerishableProduct : public Product {
protected:
    Date production_date;
    int shelf_life_days;

public:
    PerishableProduct(const std::string& id, const std::string& name, const std::string& manufacturer,
        const Price& price, const Quantity& quantity, const Weight& weight,
        const Date& production_date, int shelf_life_days);

    bool is_expired(const Date& current_date) const override;

    Date get_production_date() const;
    int get_shelf_life_days() const;
    Date get_expiration_date() const;
};