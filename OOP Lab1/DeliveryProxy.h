#pragma once
#include "Delivery.h"
#include <map>
#include <string>

class DeliveryProxy : public Delivery {
private:
    std::shared_ptr<Delivery> real_delivery;
    mutable std::map<std::string, double> cost_cache;

public:
    DeliveryProxy(std::shared_ptr<Delivery> delivery);
    double get_cost(const Order& order) const override;
    std::string get_estimated_time() const override;
    bool is_available(const Order& order) const override;
    std::string get_unavailability_reason(const Order& order) const override;
    std::shared_ptr<Delivery> clone() const override;
    DeliveryType get_type() const override { return real_delivery->get_type(); }
    std::shared_ptr<Delivery> get_real_delivery() const;
};