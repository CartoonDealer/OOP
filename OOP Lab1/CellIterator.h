#pragma once
#include <vector>
#include <memory>
#include "OrderItem.h"

class CellIterator {
private:
    using iter_type = std::vector<std::shared_ptr<OrderItem>>::iterator;
    iter_type current;

public:
    explicit CellIterator(iter_type iter) : current(iter) {}

    std::shared_ptr<OrderItem> operator*() const { return *current; }
    std::shared_ptr<OrderItem> operator->() const { return *current; }

    CellIterator& operator++() {
        ++current;
        return *this;
    }

    bool operator!=(const CellIterator& other) const { return current != other.current; }
    bool operator==(const CellIterator& other) const { return current == other.current; }
};