#include "DeliveryProxy.h"
#include "Order.h"
#include <iostream>

DeliveryProxy::DeliveryProxy(std::shared_ptr<Delivery> delivery) : real_delivery(delivery) {}

double DeliveryProxy::get_cost(const Order& order) const {
    std::string order_id = order.get_id();
    auto it = cost_cache.find(order_id);
    if (it != cost_cache.end()) {
        std::cout << "[DeliveryProxy] (Кэш) Стоимость для заказа " << order_id << " уже рассчитана.\n";
        return it->second;
    }
    double cost = real_delivery->get_cost(order);
    cost_cache[order_id] = cost;
    return cost;
}

std::string DeliveryProxy::get_estimated_time() const {
    return real_delivery->get_estimated_time();
}

bool DeliveryProxy::is_available(const Order& order) const {
    return real_delivery->is_available(order);
}

std::string DeliveryProxy::get_unavailability_reason(const Order& order) const {
    return real_delivery->get_unavailability_reason(order);
}

std::shared_ptr<Delivery> DeliveryProxy::clone() const {
    return std::make_shared<DeliveryProxy>(real_delivery);
}

std::shared_ptr<Delivery> DeliveryProxy::get_real_delivery() const {
    return real_delivery;
}