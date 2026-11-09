#include "FoodProduct.h"

FoodProduct::FoodProduct(const std::string& id, const std::string& name, const std::string& manufacturer,
    const Price& price, const Quantity& quantity, const Weight& weight,
    const Date& production_date, int shelf_life_days)
    : PerishableProduct(id, name, manufacturer, price, quantity, weight, production_date, shelf_life_days) {
}

std::unique_ptr<Product> FoodProduct::clone() const {
    return std::make_unique<FoodProduct>(*this);
}

void FoodProduct::print(std::ostream& os) const {
    os << "[Еда] ID: " << id << " | " << name
        << " (Произв: " << manufacturer << ")"
        << " | Цена: " << price.get_value() << " руб. | Кол-во: " << quantity.get_value() << " шт."
        << " | Годен до: " << get_expiration_date().to_string();
}