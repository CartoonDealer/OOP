#include "Factories.h"
#include "ProductTypes.h"

std::unique_ptr<Product> SmartphoneFactory::create_product(const std::string& id, const std::string& name,
    int price, int quantity, double weight,
    const std::string&, int) const {
    return std::make_unique<Smartphone>(id, name, Price(price), Quantity(quantity), Weight(weight));
}

std::unique_ptr<Product> LaptopFactory::create_product(const std::string& id, const std::string& name,
    int price, int quantity, double weight,
    const std::string&, int) const {
    return std::make_unique<Laptop>(id, name, Price(price), Quantity(quantity), Weight(weight));
}

std::unique_ptr<Product> TShirtFactory::create_product(const std::string& id, const std::string& name,
    int price, int quantity, double weight,
    const std::string&, int) const {
    return std::make_unique<TShirt>(id, name, Price(price), Quantity(quantity), Weight(weight));
}

std::unique_ptr<Product> JacketFactory::create_product(const std::string& id, const std::string& name,
    int price, int quantity, double weight,
    const std::string&, int) const {
    return std::make_unique<Jacket>(id, name, Price(price), Quantity(quantity), Weight(weight));
}

std::unique_ptr<Product> MilkFactory::create_product(const std::string& id, const std::string& name,
    int price, int quantity, double weight,
    const std::string& production_date, int shelf_life_days) const {
    return std::make_unique<Milk>(id, name, Price(price), Quantity(quantity), Weight(weight),
        production_date, shelf_life_days);
}

std::unique_ptr<Product> BreadFactory::create_product(const std::string& id, const std::string& name,
    int price, int quantity, double weight,
    const std::string& production_date, int shelf_life_days) const {
    return std::make_unique<Bread>(id, name, Price(price), Quantity(quantity), Weight(weight),
        production_date, shelf_life_days);
}