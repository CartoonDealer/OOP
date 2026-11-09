#pragma once
#include <string>
#include <ctime>
#include "OrderStatus.h"

class OrderEvent {
private:
    std::string order_id;
    OrderStatus old_status;
    OrderStatus new_status;
    PaymentStatus old_payment;
    PaymentStatus new_payment;
    std::string timestamp;

public:
    OrderEvent(const std::string& order_id,
        OrderStatus old_status, OrderStatus new_status,
        PaymentStatus old_payment, PaymentStatus new_payment)
        : order_id(order_id), old_status(old_status), new_status(new_status),
        old_payment(old_payment), new_payment(new_payment) {

        std::time_t now = std::time(nullptr);
        std::tm now_tm;
#ifdef _WIN32
        localtime_s(&now_tm, &now);
#else
        localtime_r(&now, &now_tm);
#endif
        char buf[20];
        std::strftime(buf, sizeof(buf), "%H:%M:%S", &now_tm);
        timestamp = buf;
    }

    std::string get_order_id() const { return order_id; }
    OrderStatus get_old_status() const { return old_status; }
    OrderStatus get_new_status() const { return new_status; }
    PaymentStatus get_old_payment() const { return old_payment; }
    PaymentStatus get_new_payment() const { return new_payment; }
    std::string get_timestamp() const { return timestamp; }
};