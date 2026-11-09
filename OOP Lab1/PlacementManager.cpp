#include "PlacementManager.h"
#include "FoodProduct.h"
#include "HouseholdChemical.h"
#include "TimeUtils.h" // <--- Для получения текущей даты
#include <stdexcept>
#include <iostream>

bool PlacementManager::is_bulky(const std::shared_ptr<Product>& product) {
    return product->get_weight().get_value() > 10.0;
}

Zone PlacementManager::determine_zone(std::shared_ptr<OrderItem> item, bool is_express) {
    if (item->get_seller_id() == "INTERNAL") return Zone::INTERNAL;

    auto product = item->get_product();

    bool is_food = dynamic_cast<FoodProduct*>(product.get()) != nullptr;
    if (is_food) return Zone::DELIVERY_FOOD;

    if (is_bulky(product)) return Zone::BIG;

    if (is_express) return Zone::DELIVERY_OTHER;

    return Zone::SHELF;
}

void PlacementManager::place_item(Warehouse& warehouse, std::shared_ptr<OrderItem> item, bool is_express) {
    if (!item) return;

    // ПРОВЕРКА НА ПРОСРОЧКУ
    if (item->get_product()->is_expired(TimeUtils::get_today())) {
        throw std::runtime_error("Нельзя добавить просроченный товар на склад: " + item->get_product()->get_name());
    }

    Zone target_zone = determine_zone(item, is_express);

    if (target_zone == Zone::INTERNAL) {
        Cell& internal_cell = warehouse.get_or_create_cell(Zone::INTERNAL, 1);
        for (auto it = internal_cell.begin(); it != internal_cell.end(); ++it) {
            auto existing_item = *it;
            if (existing_item->get_product()->get_id() == item->get_product()->get_id()) {
                existing_item->add_quantity(item->get_quantity());
                return;
            }
        }
        internal_cell.add_item(item);
        return;
    }

    int free_slot = warehouse.find_free_slot(target_zone);
    if (free_slot == -1) {
        throw std::overflow_error("Нет свободных ячеек в зоне " + get_zone_prefix(target_zone));
    }
    Cell& target_cell = warehouse.get_or_create_cell(target_zone, free_slot);
    target_cell.add_item(item);
}

void PlacementManager::move_item(Warehouse& source_wh, Warehouse& target_wh, std::shared_ptr<OrderItem> item, int quantity_to_move) {
    if (quantity_to_move <= 0) {
        throw std::invalid_argument("Количество для перемещения должно быть больше 0");
    }
    if (quantity_to_move > item->get_quantity()) {
        throw std::invalid_argument("Нельзя переместить больше товаров, чем есть на складе (в наличии: " + std::to_string(item->get_quantity()) + ")");
    }

    if (quantity_to_move == item->get_quantity()) {
        if (item->get_seller_id() == "INTERNAL") {
            Cell& internal_cell = target_wh.get_or_create_cell(Zone::INTERNAL, 1);
            for (auto it = internal_cell.begin(); it != internal_cell.end(); ++it) {
                auto existing_item = *it;
                if (existing_item->get_product()->get_id() == item->get_product()->get_id()) {
                    existing_item->add_quantity(item->get_quantity());
                    Cell* source_cell = source_wh.find_cell_with_item(item);
                    if (source_cell) source_cell->remove_item(item);
                    return;
                }
            }
            source_wh.transfer_item(target_wh, item, Zone::INTERNAL, 1);
            return;
        }

        Zone zone = determine_zone(item);
        int free_slot = target_wh.find_free_slot(zone);
        if (free_slot == -1) throw std::runtime_error("На целевом складе нет свободных ячеек!");
        source_wh.transfer_item(target_wh, item, zone, free_slot);
    }
    else {
        item->reduce_quantity(quantity_to_move);

        auto moved_item = std::make_shared<OrderItem>(
            item->get_seller_id(),
            item->get_order_id(),
            item->get_item_number(),
            item->get_product(),
            quantity_to_move
        );

        if (item->get_seller_id() == "INTERNAL") {
            Cell& internal_cell = target_wh.get_or_create_cell(Zone::INTERNAL, 1);
            for (auto it = internal_cell.begin(); it != internal_cell.end(); ++it) {
                auto existing_item = *it;
                if (existing_item->get_product()->get_id() == moved_item->get_product()->get_id()) {
                    existing_item->add_quantity(moved_item->get_quantity());
                    return;
                }
            }
            source_wh.transfer_item(target_wh, moved_item, Zone::INTERNAL, 1);
        }
        else {
            Zone zone = determine_zone(moved_item);
            int free_slot = target_wh.find_free_slot(zone);
            if (free_slot == -1) throw std::runtime_error("На целевом складе нет свободных ячеек!");
            source_wh.transfer_item(target_wh, moved_item, zone, free_slot);
        }
    }
}