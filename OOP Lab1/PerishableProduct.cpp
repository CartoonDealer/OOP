#include "PerishableProduct.h"
#include <stdexcept>

PerishableProduct::PerishableProduct(const std::string& id, const std::string& name, const std::string& manufacturer,
    const Price& price, const Quantity& quantity, const Weight& weight,
    const Date& production_date, int shelf_life_days)
    : Product(id, name, manufacturer, price, quantity, weight),
    production_date(production_date), shelf_life_days(shelf_life_days) {
    if (shelf_life_days < 0) {
        throw std::invalid_argument("Срок годности не может быть отрицательным");
    }
}

bool PerishableProduct::is_expired(const Date& current_date) const {
    Date expiration_date = production_date.add_days(shelf_life_days);
    return expiration_date < current_date;
}

Date PerishableProduct::get_production_date() const { return production_date; }
int PerishableProduct::get_shelf_life_days() const { return shelf_life_days; }
Date PerishableProduct::get_expiration_date() const { return production_date.add_days(shelf_life_days); }