#pragma once
#include <vector>
#include <memory>
#include "Product.h"

class WarehouseIterator {
private:
    using iter_type = std::vector<std::unique_ptr<Product>>::iterator;
    iter_type current;
    iter_type end;

public:
    WarehouseIterator(iter_type curr, iter_type e) : current(curr), end(e) {}

    Product& operator*() const { return **current; }
    Product* operator->() const { return current->get(); }

    WarehouseIterator& operator++() {
        ++current;
        return *this;
    }

    bool operator!=(const WarehouseIterator& other) const { return current != other.current; }
    bool operator==(const WarehouseIterator& other) const { return current == other.current; }
};

class ConstWarehouseIterator {
private:
    using iter_type = std::vector<std::unique_ptr<Product>>::const_iterator;
    iter_type current;
    iter_type end;

public:
    ConstWarehouseIterator(iter_type curr, iter_type e) : current(curr), end(e) {}

    const Product& operator*() const { return **current; }
    const Product* operator->() const { return current->get(); }

    ConstWarehouseIterator& operator++() {
        ++current;
        return *this;
    }

    bool operator!=(const ConstWarehouseIterator& other) const { return current != other.current; }
    bool operator==(const ConstWarehouseIterator& other) const { return current == other.current; }
};