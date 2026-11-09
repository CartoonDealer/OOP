#include "Cell.h"
#include <stdexcept>
#include <algorithm>

Cell::Cell(Zone zone, int number) : zone(zone), number(number) {}

Zone Cell::get_zone() const { return zone; }
int Cell::get_number() const { return number; }
std::string Cell::get_seller_id() const { return seller_id; }
std::string Cell::get_order_id() const { return order_id; }
bool Cell::is_empty() const { return items.empty(); }

std::string Cell::get_location_string() const {
    return get_zone_prefix(zone) + "-" + std::to_string(number);
}

bool Cell::can_accept(std::shared_ptr<OrderItem> item) const {
    if (is_empty()) return true;

    // 1. Если товар в заказе (order_id не пустой), главное — совпадение заказа.
    // Продавцы могут быть разные (ИП Петров + Подарок от Магазина).
    if (!item->get_order_id().empty()) {
        return order_id == item->get_order_id();
    }
    // 2. Если товар свободный (order_id пустой), главное — совпадение продавца.
    if (item->get_order_id().empty()) {
        return order_id.empty() && seller_id == item->get_seller_id();
    }
    return false;
}

void Cell::add_item(std::shared_ptr<OrderItem> item) {
    if (!can_accept(item)) {
        throw std::runtime_error("Нельзя смешивать разные заказы или разных продавцов в одной ячейке!");
    }

    if (is_empty()) {
        seller_id = item->get_seller_id();
        order_id = item->get_order_id();
    }

    item->set_location(get_location_string());
    items.push_back(item);
}

void Cell::remove_item(std::shared_ptr<OrderItem> item) {
    auto it = std::find(items.begin(), items.end(), item);
    if (it != items.end()) {
        items.erase(it);
        if (items.empty()) {
            seller_id = "";
            order_id = "";
        }
    }
}

bool Cell::contains_item(std::shared_ptr<OrderItem> item) const {
    return std::find(items.begin(), items.end(), item) != items.end();
}