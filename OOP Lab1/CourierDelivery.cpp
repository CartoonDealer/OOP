#include "CourierDelivery.h"
#include "Order.h"
#include "Customer.h"
#include <cmath>
#include <sstream>
#include <iomanip>

double CourierDelivery::get_cost(const Order& order) const {
    double base = 500.0;
    double weight = order.get_total_weight();
    if (weight > 5.0) {
        int extra_kg = static_cast<int>(std::ceil(weight - 5.0));
        return base + (extra_kg * 50.0);
    }
    return base;
}

std::string CourierDelivery::get_estimated_time() const {
    return "В течение дня"; // <--- ИЗМЕНЕНО
}

bool CourierDelivery::is_available(const Order& order) const {
    return get_unavailability_reason(order).empty();
}

std::string CourierDelivery::get_unavailability_reason(const Order& order) const {
    std::string region = order.get_customer()->get_region();
    if (region != "СПб" && region != "Санкт-Петербург") {
        return "Курьерская доставка недоступна. Мы не доставляем в регион: " + region;
    }
    if (order.get_total_weight() > 20.0) {
        std::ostringstream ss;
        ss << "Курьер не может доставить заказ. Превышен вес: " << std::fixed << std::setprecision(1) << order.get_total_weight() << " кг (максимум 20 кг).";
        return ss.str();
    }
    return "";
}

std::shared_ptr<Delivery> CourierDelivery::clone() const {
    return std::make_shared<CourierDelivery>(*this);
}