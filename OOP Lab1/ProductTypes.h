#pragma once
#include "Product.h"
#include <memory>
#include <iostream>
#include <string>

class Electronics : public Product {
public:
    Electronics(const std::string& id, const std::string& name, const Price& price,
        const Quantity& quantity, const Weight& weight)
        : Product(id, name, price, quantity, weight, "electronics") {
    }
};

class Clothing : public Product {
public:
    Clothing(const std::string& id, const std::string& name, const Price& price,
        const Quantity& quantity, const Weight& weight)
        : Product(id, name, price, quantity, weight, "clothing") {
    }
};

class Food : public Product {
protected:
    std::string production_date;
    int shelf_life_days;
    std::string calculate_expiration_date() const;

public:
    Food(const std::string& id, const std::string& name, const Price& price,
        const Quantity& quantity, const Weight& weight,
        const std::string& prod_date, int shelf_life)
        : Product(id, name, price, quantity, weight, "food"),
        production_date(prod_date), shelf_life_days(shelf_life) {
    }
};

class Smartphone : public Electronics {
public:
    Smartphone(const std::string& id, const std::string& name, const Price& price,
        const Quantity& quantity, const Weight& weight);
    std::unique_ptr<Product> clone() const override;
    void print(std::ostream& os) const override;
    void transfer_to(Warehouse& other) const override;
};

class Laptop : public Electronics {
public:
    Laptop(const std::string& id, const std::string& name, const Price& price,
        const Quantity& quantity, const Weight& weight);
    std::unique_ptr<Product> clone() const override;
    void print(std::ostream& os) const override;
    void transfer_to(Warehouse& other) const override;
};

class TShirt : public Clothing {
public:
    TShirt(const std::string& id, const std::string& name, const Price& price,
        const Quantity& quantity, const Weight& weight);
    std::unique_ptr<Product> clone() const override;
    void print(std::ostream& os) const override;
    void transfer_to(Warehouse& other) const override;
};

class Jacket : public Clothing {
public:
    Jacket(const std::string& id, const std::string& name, const Price& price,
        const Quantity& quantity, const Weight& weight);
    std::unique_ptr<Product> clone() const override;
    void print(std::ostream& os) const override;
    void transfer_to(Warehouse& other) const override;
};

class Milk : public Food {
public:
    Milk(const std::string& id, const std::string& name, const Price& price,
        const Quantity& quantity, const Weight& weight,
        const std::string& prod_date, int shelf_life);
    std::unique_ptr<Product> clone() const override;
    void print(std::ostream& os) const override;
    void transfer_to(Warehouse& other) const override;
};

class Bread : public Food {
public:
    Bread(const std::string& id, const std::string& name, const Price& price,
        const Quantity& quantity, const Weight& weight,
        const std::string& prod_date, int shelf_life);
    std::unique_ptr<Product> clone() const override;
    void print(std::ostream& os) const override;
    void transfer_to(Warehouse& other) const override;
};