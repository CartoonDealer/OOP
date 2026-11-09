#include "StandardFactory.h"

std::shared_ptr<Product> StandardFactory::create_product(const std::string& id, const std::string& name,
    const std::string& manufacturer,
    int price, int quantity, double weight) const {
    return std::make_shared<StandardProduct>(id, name, manufacturer, Price(price), Quantity(quantity), Weight(weight));
}