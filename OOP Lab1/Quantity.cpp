#include "Quantity.h"
#include <stdexcept>

Quantity::Quantity() : value(0) {}

Quantity::Quantity(int val) : value(val) {
    if (val < 0) {
        throw std::invalid_argument(" оличество не может быть отрицательным");
    }
}

int Quantity::get_value() const {
    return value;
}

void Quantity::set_value(int new_val) {
    if (new_val < 0) {
        throw std::invalid_argument(" оличество не может быть отрицательным");
    }
    value = new_val;
}

void Quantity::change(int delta) {
    int new_val = value + delta;
    if (new_val < 0) {
        throw std::invalid_argument(" оличество не может стать отрицательным");
    }
    value = new_val;
}