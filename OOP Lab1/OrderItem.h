#pragma once
#include <string>
#include <memory>
#include "Product.h"

class OrderItem {
private:
    std::string seller_id;
    std::string order_id;
    int item_number;
    std::shared_ptr<Product> product;
    int quantity;
    std::string location;

public:
    OrderItem(const std::string& seller_id, const std::string& order_id, int item_number,
        std::shared_ptr<Product> product, int quantity);

    std::string get_seller_id() const;
    std::string get_order_id() const;
    int get_item_number() const;
    std::shared_ptr<Product> get_product() const;
    int get_quantity() const;
    std::string get_location() const;

    void set_order_id(const std::string& id); 
    void set_location(const std::string& loc);
    void add_quantity(int amount);
    void reduce_quantity(int amount);
};