#pragma once
#include <map>
#include <memory>
#include <utility>
#include <string>
#include "Cell.h"
#include "WarehouseIterator.h"
#include "Zone.h"

class Warehouse {
private:
    std::map<std::pair<Zone, int>, std::shared_ptr<Cell>> cells;
    std::string address;
    int total_cells;

public:
    explicit Warehouse(const std::string& address = "Не указан", int initial_cells = 10);

    std::string get_address() const;
    int get_total_cells() const;
    void add_cells(int count);

    Cell& get_or_create_cell(Zone zone, int slot_number);
    size_t get_occupied_cells_count() const;

    int find_free_slot(Zone zone) const;

    Cell* find_cell_with_item(std::shared_ptr<OrderItem> item);
    bool transfer_item(Warehouse& target_wh, std::shared_ptr<OrderItem> item, Zone target_zone, int target_slot);

    // НОВЫЙ МЕТОД: удаляет все товары конкретного заказа
    void remove_order_items(const std::string& order_id);

    WarehouseIterator begin() { return WarehouseIterator(cells.begin()); }
    WarehouseIterator end() { return WarehouseIterator(cells.end()); }
};