#include "Weight.h"
#include <stdexcept>

Weight::Weight() : value(0.0) {}

Weight::Weight(double val) : value(val) {
    if (val < 0.0) {
        throw std::invalid_argument("Вес не может быть отрицательным");
    }
}

double Weight::get_value() const {
    return value;
}

void Weight::set_value(double new_val) {
    if (new_val < 0.0) {
        throw std::invalid_argument("Вес не может быть отрицательным");
    }
    value = new_val;
}