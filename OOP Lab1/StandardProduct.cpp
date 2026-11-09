#include "StandardProduct.h"

StandardProduct::StandardProduct(const std::string& id, const std::string& name, const std::string& manufacturer,
    const Price& price, const Quantity& quantity, const Weight& weight)
    : Product(id, name, manufacturer, price, quantity, weight) {
}

std::unique_ptr<Product> StandardProduct::clone() const {
    return std::make_unique<StandardProduct>(*this);
}

void StandardProduct::print(std::ostream& os) const {
    os << "[Обычный товар] ID: " << id << " | " << name
        << " (Произв: " << manufacturer << ")"
        << " | Цена: " << price.get_value() << " руб. | Кол-во: " << quantity.get_value() << " шт. | Вес: " << weight.get_value() << " кг";
}