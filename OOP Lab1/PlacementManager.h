#pragma once
#include <memory>
#include "Warehouse.h"
#include "OrderItem.h"

class PlacementManager {
private:
    static bool is_bulky(const std::shared_ptr<Product>& product);

public:
    static Zone determine_zone(std::shared_ptr<OrderItem> item, bool is_express = false);
    static void place_item(Warehouse& warehouse, std::shared_ptr<OrderItem> item, bool is_express = false);

    // Обновленный метод: принимает количество для перемещения
    static void move_item(Warehouse& source_wh, Warehouse& target_wh, std::shared_ptr<OrderItem> item, int quantity_to_move);
};