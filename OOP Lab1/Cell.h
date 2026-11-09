#pragma once
#include <vector>
#include <memory>
#include <string>
#include "OrderItem.h"
#include "Zone.h"
#include "CellIterator.h"

class Cell {
private:
    Zone zone;
    int number;
    std::string seller_id;
    std::string order_id;
    std::vector<std::shared_ptr<OrderItem>> items;

public:
    Cell(Zone zone, int number);

    Zone get_zone() const;
    int get_number() const;
    std::string get_location_string() const;
    bool is_empty() const;

    std::string get_seller_id() const;
    std::string get_order_id() const;

    void add_item(std::shared_ptr<OrderItem> item);
    void remove_item(std::shared_ptr<OrderItem> item);
    bool contains_item(std::shared_ptr<OrderItem> item) const;

    bool can_accept(std::shared_ptr<OrderItem> item) const;

    CellIterator begin() { return CellIterator(items.begin()); }
    CellIterator end() { return CellIterator(items.end()); }
};