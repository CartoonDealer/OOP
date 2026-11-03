#pragma once

#include <string>
#include <memory>
#include <iostream>
#include "Price.h"
#include "Quantity.h"
#include "Weight.h"

// Предварительное объявление для разрыва циклической зависимости
class Warehouse;

class Product {
protected:
    std::string id;
    std::string name;
    Price price;
    Quantity quantity;
    Weight weight;
    std::string category;

    static int next_id;

public:
    Product(const std::string& id, const std::string& name, const Price& price,
        const Quantity& quantity, const Weight& weight,
        const std::string& category);

    virtual ~Product() = default;

    static std::string generate_id();

    std::string get_id() const;
    std::string get_name() const;
    Price get_price() const;
    Quantity get_quantity() const;
    Weight get_weight() const;
    std::string get_category() const;

    void set_price(const Price& new_price);
    void change_quantity(int delta);

    virtual void transfer_to(Warehouse& other) const = 0;

    virtual std::unique_ptr<Product> clone() const = 0;
    virtual void print(std::ostream& os) const = 0;
};