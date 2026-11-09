#include "FoodFactory.h"

std::shared_ptr<Product> FoodFactory::create_product(const std::string& id, const std::string& name,
    const std::string& manufacturer,
    int price, int quantity, double weight,
    const Date& production_date, int shelf_life_days) const {
    return std::make_shared<FoodProduct>(id, name, manufacturer, Price(price), Quantity(quantity), Weight(weight),
        production_date, shelf_life_days);
}