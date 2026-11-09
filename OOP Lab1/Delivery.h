#pragma once
#include <string>
#include <memory>

class Order;

enum class DeliveryType {
    COURIER,
    POST,
    PICKUP
};

class Delivery {
public:
    virtual ~Delivery() = default;

    virtual double get_cost(const Order& order) const = 0;
    virtual std::string get_estimated_time() const = 0;
    virtual bool is_available(const Order& order) const = 0;

    // НОВЫЙ МЕТОД: Возвращает причину, по которой доставка недоступна (или пустую строку)
    virtual std::string get_unavailability_reason(const Order& order) const = 0;

    virtual std::shared_ptr<Delivery> clone() const = 0;
    virtual DeliveryType get_type() const = 0;
};

Order& operator<<(const Delivery& delivery, Order& order);