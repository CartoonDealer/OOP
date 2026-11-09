#pragma once
#include "Delivery.h"

class CourierDelivery : public Delivery {
public:
    double get_cost(const Order& order) const override;
    std::string get_estimated_time() const override;
    bool is_available(const Order& order) const override;
    std::string get_unavailability_reason(const Order& order) const override;
    std::shared_ptr<Delivery> clone() const override;
    DeliveryType get_type() const override { return DeliveryType::COURIER; }
};