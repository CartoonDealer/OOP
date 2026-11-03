#include "ProductTypes.h"
#include "Warehouse.h"
#include <iomanip>
#include <sstream>
#include <ctime>

std::string Food::calculate_expiration_date() const {
    if (production_date.empty() || shelf_life_days <= 0) return "Не указан";

    std::tm tm = {};
    std::istringstream ss(production_date);
    ss >> std::get_time(&tm, "%d.%m.%Y");

    if (ss.fail()) return "Ошибка формата даты";

    tm.tm_mday += shelf_life_days;
    std::mktime(&tm);

    std::ostringstream os;
    os << std::put_time(&tm, "%d.%m.%Y");
    return os.str();
}

Smartphone::Smartphone(const std::string& id, const std::string& name, const Price& price,
    const Quantity& quantity, const Weight& weight)
    : Electronics(id, name, price, quantity, weight) {
}

std::unique_ptr<Product> Smartphone::clone() const { return std::make_unique<Smartphone>(*this); }
void Smartphone::transfer_to(Warehouse& other) const { other.add_product(this->clone()); }
void Smartphone::print(std::ostream& os) const {
    os << "[Смартфон] ID: " << id << " | " << name
        << " | Цена: " << price.get_value() << " руб. | Кол-во: " << quantity.get_value() << " шт. | Вес: " << weight.get_value() << " г";
}

Laptop::Laptop(const std::string& id, const std::string& name, const Price& price,
    const Quantity& quantity, const Weight& weight)
    : Electronics(id, name, price, quantity, weight) {
}

std::unique_ptr<Product> Laptop::clone() const { return std::make_unique<Laptop>(*this); }
void Laptop::transfer_to(Warehouse& other) const { other.add_product(this->clone()); }
void Laptop::print(std::ostream& os) const {
    os << "[Ноутбук] ID: " << id << " | " << name
        << " | Цена: " << price.get_value() << " руб. | Кол-во: " << quantity.get_value() << " шт. | Вес: " << weight.get_value() << " г";
}

TShirt::TShirt(const std::string& id, const std::string& name, const Price& price,
    const Quantity& quantity, const Weight& weight)
    : Clothing(id, name, price, quantity, weight) {
}

std::unique_ptr<Product> TShirt::clone() const { return std::make_unique<TShirt>(*this); }
void TShirt::transfer_to(Warehouse& other) const { other.add_product(this->clone()); }
void TShirt::print(std::ostream& os) const {
    os << "[Футболка] ID: " << id << " | " << name
        << " | Цена: " << price.get_value() << " руб. | Кол-во: " << quantity.get_value() << " шт. | Вес: " << weight.get_value() << " г";
}

Jacket::Jacket(const std::string& id, const std::string& name, const Price& price,
    const Quantity& quantity, const Weight& weight)
    : Clothing(id, name, price, quantity, weight) {
}

std::unique_ptr<Product> Jacket::clone() const { return std::make_unique<Jacket>(*this); }
void Jacket::transfer_to(Warehouse& other) const { other.add_product(this->clone()); }
void Jacket::print(std::ostream& os) const {
    os << "[Куртка] ID: " << id << " | " << name
        << " | Цена: " << price.get_value() << " руб. | Кол-во: " << quantity.get_value() << " шт. | Вес: " << weight.get_value() << " г";
}

Milk::Milk(const std::string& id, const std::string& name, const Price& price,
    const Quantity& quantity, const Weight& weight,
    const std::string& prod_date, int shelf_life)
    : Food(id, name, price, quantity, weight, prod_date, shelf_life) {
}

std::unique_ptr<Product> Milk::clone() const { return std::make_unique<Milk>(*this); }
void Milk::transfer_to(Warehouse& other) const { other.add_product(this->clone()); }
void Milk::print(std::ostream& os) const {
    os << "[Молоко] ID: " << id << " | " << name
        << " | Цена: " << price.get_value() << " руб. | Кол-во: " << quantity.get_value() << " шт. | Вес: " << weight.get_value() << " г"
        << " | Произведено: " << production_date << " | Годен до: " << calculate_expiration_date() << " (" << shelf_life_days << " дн.)";
}

Bread::Bread(const std::string& id, const std::string& name, const Price& price,
    const Quantity& quantity, const Weight& weight,
    const std::string& prod_date, int shelf_life)
    : Food(id, name, price, quantity, weight, prod_date, shelf_life) {
}

std::unique_ptr<Product> Bread::clone() const { return std::make_unique<Bread>(*this); }
void Bread::transfer_to(Warehouse& other) const { other.add_product(this->clone()); }
void Bread::print(std::ostream& os) const {
    os << "[Хлеб] ID: " << id << " | " << name
        << " | Цена: " << price.get_value() << " руб. | Кол-во: " << quantity.get_value() << " шт. | Вес: " << weight.get_value() << " г"
        << " | Произведено: " << production_date << " | Годен до: " << calculate_expiration_date() << " (" << shelf_life_days << " дн.)";
}