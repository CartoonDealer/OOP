#pragma once
#include <string>
#include <memory>
#include <iostream>
#include "Price.h"
#include "Quantity.h"
#include "Weight.h"
#include "Date.h"

class Product {
protected:
    std::string id;
    std::string name;
    std::string manufacturer; // НОВОЕ ПОЛЕ: Чей это товар
    Price price;
    Quantity quantity;
    Weight weight;

public:
    Product(const std::string& id, const std::string& name, const std::string& manufacturer,
        const Price& price, const Quantity& quantity, const Weight& weight);

    virtual ~Product() = default;

    std::string get_id() const;
    std::string get_name() const;
    std::string get_manufacturer() const; // Геттер
    Price get_price() const;
    Quantity get_quantity() const;
    Weight get_weight() const;

    void set_price(const Price& new_price);
    void change_quantity(int delta);

    virtual bool is_expired(const Date& current_date) const { return false; }
    virtual std::unique_ptr<Product> clone() const = 0;
    virtual void print(std::ostream& os) const = 0;
};