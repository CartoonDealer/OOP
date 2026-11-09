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
    std::string customer_id;
    std::vector<std::shared_ptr<OrderItem>> items;

public:
    Cell(Zone zone, int number);

    Zone get_zone() const;
    int get_number() const;
    std::string get_location_string() const;
    std::string get_customer_id() const;
    bool is_empty() const;

    void add_item(std::shared_ptr<OrderItem> item, const std::string& cust_id);
    void print() const;

    // »тераторы дл€ обхода товаров внутри €чейки
    CellIterator begin() { return CellIterator(items.begin()); }
    CellIterator end() { return CellIterator(items.end()); }
};