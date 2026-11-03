#pragma once
#include "Product.h"
#include <memory>
#include <string>

class ProductFactory {
public:
    virtual ~ProductFactory() = default;

    virtual std::unique_ptr<Product> create_product(const std::string& id, const std::string& name,
        int price, int quantity, double weight,
        const std::string& production_date = "",
        int shelf_life_days = 0) const = 0;
};

class SmartphoneFactory : public ProductFactory {
public:
    std::unique_ptr<Product> create_product(const std::string& id, const std::string& name,
        int price, int quantity, double weight,
        const std::string& production_date = "",
        int shelf_life_days = 0) const override;
};

class LaptopFactory : public ProductFactory {
public:
    std::unique_ptr<Product> create_product(const std::string& id, const std::string& name,
        int price, int quantity, double weight,
        const std::string& production_date = "",
        int shelf_life_days = 0) const override;
};

class TShirtFactory : public ProductFactory {
public:
    std::unique_ptr<Product> create_product(const std::string& id, const std::string& name,
        int price, int quantity, double weight,
        const std::string& production_date = "",
        int shelf_life_days = 0) const override;
};

class JacketFactory : public ProductFactory {
public:
    std::unique_ptr<Product> create_product(const std::string& id, const std::string& name,
        int price, int quantity, double weight,
        const std::string& production_date = "",
        int shelf_life_days = 0) const override;
};

class MilkFactory : public ProductFactory {
public:
    std::unique_ptr<Product> create_product(const std::string& id, const std::string& name,
        int price, int quantity, double weight,
        const std::string& production_date = "",
        int shelf_life_days = 0) const override;
};

class BreadFactory : public ProductFactory {
public:
    std::unique_ptr<Product> create_product(const std::string& id, const std::string& name,
        int price, int quantity, double weight,
        const std::string& production_date = "",
        int shelf_life_days = 0) const override;
};