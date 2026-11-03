#include "Product.h"
#include "Warehouse.h"
#include <stdexcept>

int Product::next_id = 1;

Product::Product(const std::string& id, const std::string& name, const Price& price,
    const Quantity& quantity, const Weight& weight,
    const std::string& category)
    : id(id), name(name), price(price), quantity(quantity), weight(weight), category(category) {
}

std::string Product::generate_id() {
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "460123400%05d", next_id++);
    return std::string(buffer);
}

std::string Product::get_id() const { return id; }
std::string Product::get_name() const { return name; }
Price Product::get_price() const { return price; }
Quantity Product::get_quantity() const { return quantity; }
Weight Product::get_weight() const { return weight; }
std::string Product::get_category() const { return category; }

void Product::set_price(const Price& new_price) {
    this->price = new_price;
}

void Product::change_quantity(int delta) {
    this->quantity.change(delta);
}