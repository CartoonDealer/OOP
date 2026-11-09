#include "Warehouse.h"
#include <stdexcept>

Warehouse::Warehouse(const std::string& address, int initial_cells)
    : address(address), total_cells(initial_cells) {
    if (initial_cells <= 0) throw std::invalid_argument("Вместимость склада должна быть больше 0");
}

std::string Warehouse::get_address() const { return address; }
int Warehouse::get_total_cells() const { return total_cells; }

void Warehouse::add_cells(int count) {
    if (count > 0) total_cells += count;
}

Cell& Warehouse::get_or_create_cell(Zone zone, int slot_number) {
    auto key = std::make_pair(zone, slot_number);
    auto it = cells.find(key);

    if (it == cells.end()) {
        auto new_cell = std::make_shared<Cell>(zone, slot_number);
        cells[key] = new_cell;
        return *new_cell;
    }
    return *(it->second);
}

size_t Warehouse::get_occupied_cells_count() const {
    size_t count = 0;
    for (const auto& pair : cells) {
        if (pair.first.first == Zone::INTERNAL) continue;
        if (!pair.second->is_empty()) count++;
    }
    return count;
}

int Warehouse::find_free_slot(Zone zone) const {
    if (zone == Zone::INTERNAL) return 1;

    for (int i = 1; i <= total_cells; ++i) {
        auto key = std::make_pair(zone, i);
        auto it = cells.find(key);
        if (it == cells.end() || it->second->is_empty()) {
            return i;
        }
    }
    return -1;
}

Cell* Warehouse::find_cell_with_item(std::shared_ptr<OrderItem> item) {
    for (auto& pair : cells) {
        if (pair.second->contains_item(item)) {
            return pair.second.get();
        }
    }
    return nullptr;
}

// НОВЫЙ МЕТОД
void Warehouse::remove_order_items(const std::string& order_id) {
    for (auto& pair : cells) {
        auto& cell = pair.second;
        std::vector<std::shared_ptr<OrderItem>> to_remove;

        for (auto it = cell->begin(); it != cell->end(); ++it) {
            if ((*it)->get_order_id() == order_id) {
                to_remove.push_back(*it);
            }
        }

        for (auto& item : to_remove) {
            cell->remove_item(item);
        }
    }
}

bool Warehouse::transfer_item(Warehouse& target_wh, std::shared_ptr<OrderItem> item, Zone target_zone, int target_slot) {
    if (!item) throw std::invalid_argument("Товар не может быть nullptr");

    if (target_zone != Zone::INTERNAL) {
        if (target_wh.get_occupied_cells_count() >= (size_t)target_wh.get_total_cells()) {
            auto key = std::make_pair(target_zone, target_slot);
            if (target_wh.cells.find(key) == target_wh.cells.end()) {
                throw std::overflow_error("На целевом складе нет свободных ячеек!");
            }
        }
    }

    auto target_key = std::make_pair(target_zone, target_slot);
    auto it_target = target_wh.cells.find(target_key);

    if (it_target != target_wh.cells.end()) {
        if (!it_target->second->can_accept(item)) {
            throw std::runtime_error("Целевая ячейка занята другим продавцом или заказом");
        }
    }

    Cell& target_cell = target_wh.get_or_create_cell(target_zone, target_slot);
    target_cell.add_item(item);

    Cell* source_cell = find_cell_with_item(item);
    if (source_cell) {
        source_cell->remove_item(item);
    }

    return true;
}