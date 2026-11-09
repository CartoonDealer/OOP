#pragma once
#include <map>
#include <memory>
#include <utility>
#include "Cell.h"
#include "Zone.h"

class WarehouseIterator {
private:
    using map_iter_type = std::map<std::pair<Zone, int>, std::shared_ptr<Cell>>::iterator;
    map_iter_type current;

public:
    explicit WarehouseIterator(map_iter_type iter) : current(iter) {}

    // Разыменование возвращает ссылку на ячейку (а не на пару ключ-значение)
    Cell& operator*() const { return *(current->second); }

    // Доступ к методам ячейки через указатель
    Cell* operator->() const { return current->second.get(); }

    WarehouseIterator& operator++() {
        ++current;
        return *this;
    }

    bool operator!=(const WarehouseIterator& other) const { return current != other.current; }
    bool operator==(const WarehouseIterator& other) const { return current == other.current; }
};