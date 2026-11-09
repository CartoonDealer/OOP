#include "PostDelivery.h"
#include "Order.h"
#include <cmath>
#include <sstream>
#include <iomanip>

double PostDelivery::get_cost(const Order& order) const {
    double base = 200.0;
    double weight = order.get_total_weight();
    if (weight > 5.0) {
        int extra_kg = static_cast<int>(std::ceil(weight - 5.0));
        return base + (extra_kg * 30.0);
    }
    return base;
}

std::string PostDelivery::get_estimated_time() const { return "5-10 дней"; }

bool PostDelivery::is_available(const Order& order) const {
    return get_unavailability_reason(order).empty();
}

std::string PostDelivery::get_unavailability_reason(const Order& order) const {
    if (order.get_total_weight() > 10.0) {
        std::ostringstream ss;
        ss << "Почта не может принять заказ. Превышен вес: " << std::fixed << std::setprecision(1) << order.get_total_weight() << " кг (максимум 10 кг).";
        return ss.str();
    }
    return "";
}

std::shared_ptr<Delivery> PostDelivery::clone() const {
    return std::make_shared<PostDelivery>(*this);
}