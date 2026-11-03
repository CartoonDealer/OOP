#include "Price.h"
#include <stdexcept>

Price::Price() : value(0) {}

Price::Price(int val) : value(val) {
    if (val < 0) {
        throw std::invalid_argument("Цена не может быть отрицательной");
    }
}

int Price::get_value() const {
    return value;
}

void Price::set_value(int new_val) {
    if (new_val < 0) {
        throw std::invalid_argument("Цена не может быть отрицательной");
    }
    value = new_val;
}