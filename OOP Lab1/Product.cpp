#include "Product.h"

Product::Product(const std::string& id, const std::string& name, const std::string& manufacturer,
    const Price& price, const Quantity& quantity, const Weight& weight)
    : id(id), name(name), manufacturer(manufacturer), price(price), quantity(quantity), weight(weight) {
}

std::string Product::get_id() const { return id; }
std::string Product::get_name() const { return name; }
std::string Product::get_manufacturer() const { return manufacturer; }
Price Product::get_price() const { return price; }
Quantity Product::get_quantity() const { return quantity; }
Weight Product::get_weight() const { return weight; }

void Product::set_price(const Price& new_price) { this->price = new_price; }
void Product::change_quantity(int delta) { this->quantity.change(delta); }