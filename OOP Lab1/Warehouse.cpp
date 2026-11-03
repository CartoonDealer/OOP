#include "Warehouse.h"
#include <stdexcept>
#include <algorithm>

Warehouse::Warehouse(size_t maxSize) : max_size(maxSize) {
    if (maxSize == 0) {
        throw std::invalid_argument("Вместимость склада должна быть больше 0");
    }
    products.reserve(max_size);
}

Warehouse::Warehouse(const Warehouse& other) : max_size(other.max_size) {
    products.reserve(max_size);
    for (const auto& p : other.products) {
        products.push_back(p->clone());
    }
}

Warehouse& Warehouse::operator=(const Warehouse& other) {
    if (this != &other) {
        products.clear();
        products.reserve(other.max_size);
        for (const auto& p : other.products) {
            products.push_back(p->clone());
        }
        max_size = other.max_size;
    }
    return *this;
}

Warehouse::Warehouse(Warehouse&& other) noexcept
    : products(std::move(other.products)), max_size(other.max_size) {
    other.max_size = 0;
}

Warehouse& Warehouse::operator=(Warehouse&& other) noexcept {
    if (this != &other) {
        products = std::move(other.products);
        max_size = other.max_size;
        other.max_size = 0;
    }
    return *this;
}

void Warehouse::add_product(std::unique_ptr<Product> product) {
    if (!product) {
        throw std::invalid_argument("Нельзя добавить пустой товар (nullptr)");
    }
    if (products.size() >= max_size) {
        throw std::overflow_error("Превышена максимальная вместимость склада");
    }
    if (find_by_id(product->get_id()) != nullptr) {
        throw std::invalid_argument("Товар с таким ID уже существует на складе");
    }
    products.push_back(std::move(product));
}

void Warehouse::remove_by_id(const std::string& id) {
    auto it = std::remove_if(products.begin(), products.end(),
        [&id](const std::unique_ptr<Product>& p) { return p->get_id() == id; });

    if (it == products.end()) {
        throw std::runtime_error("Товар с таким ID не найден");
    }
    products.erase(it, products.end());
}

void Warehouse::remove_by_index(size_t index) {
    if (index >= products.size()) {
        throw std::out_of_range("Индекс вне диапазона (товара с таким индексом нет)");
    }
    products.erase(products.begin() + index);
}

Product* Warehouse::find_by_id(const std::string& id) {
    for (const auto& p : products) {
        if (p->get_id() == id) return p.get();
    }
    return nullptr;
}

std::vector<Product*> Warehouse::find_by_category(const std::string& category) {
    std::vector<Product*> result;
    for (const auto& p : products) {
        if (p->get_category() == category) {
            result.push_back(p.get());
        }
    }
    return result;
}

std::vector<Product*> Warehouse::find_by_price(const Price& price) {
    std::vector<Product*> result;
    for (const auto& p : products) {
        if (p->get_price().get_value() == price.get_value()) {
            result.push_back(p.get());
        }
    }
    return result;
}