#include "OrderItem.h"
#include <stdexcept>

OrderItem::OrderItem(const std::string& seller_id, const std::string& order_id, int item_number,
    std::shared_ptr<Product> product, int quantity)
    : seller_id(seller_id), order_id(order_id), item_number(item_number),
    product(product), quantity(quantity), location("INBOUND") {
}

std::string OrderItem::get_seller_id() const { return seller_id; }
std::string OrderItem::get_order_id() const { return order_id; }
int OrderItem::get_item_number() const { return item_number; }
std::shared_ptr<Product> OrderItem::get_product() const { return product; }
int OrderItem::get_quantity() const { return quantity; }
std::string OrderItem::get_location() const { return location; }

void OrderItem::set_order_id(const std::string& id) {
    order_id = id;
}

void OrderItem::set_location(const std::string& loc) {
    location = loc;
}

void OrderItem::add_quantity(int amount) {
    if (amount > 0) quantity += amount;
}

void OrderItem::reduce_quantity(int amount) {
    if (amount <= 0) throw std::invalid_argument(" оличество дл€ списани€ должно быть больше 0");
    if (amount > quantity) throw std::invalid_argument("Ќельз€ списать больше, чем есть на складе");
    quantity -= amount;
}