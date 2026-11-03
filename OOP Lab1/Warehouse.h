#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Product.h"
#include "WarehouseIterator.h"

class Warehouse {
private:
    std::vector<std::unique_ptr<Product>> products;
    size_t max_size;

public:
    explicit Warehouse(size_t maxSize);

    Warehouse(const Warehouse& other);                 
    Warehouse& operator=(const Warehouse& other);      
    Warehouse(Warehouse&& other) noexcept;             
    Warehouse& operator=(Warehouse&& other) noexcept;  
    ~Warehouse() = default;                            

    void add_product(std::unique_ptr<Product> product);
    void remove_by_id(const std::string& id);
    void remove_by_index(size_t index);

    Product* find_by_id(const std::string& id);
    std::vector<Product*> find_by_category(const std::string& category);
    std::vector<Product*> find_by_price(const Price& price);

    size_t size() const { return products.size(); }
    size_t capacity() const { return max_size; }

    WarehouseIterator begin() { return WarehouseIterator(products.begin(), products.end()); }
    WarehouseIterator end() { return WarehouseIterator(products.end(), products.end()); }

    ConstWarehouseIterator begin() const { return ConstWarehouseIterator(products.cbegin(), products.cend()); }
    ConstWarehouseIterator end() const { return ConstWarehouseIterator(products.cend(), products.cend()); }
};