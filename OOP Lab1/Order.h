#pragma once
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "OrderItem.h"
#include "Customer.h"
#include "OrderStatus.h"
#include "IObservable.h"
#include "Delivery.h"
#include "Discount.h"

class Order : public IObservable {
private:
    std::string id;
    std::shared_ptr<Customer> customer;
    std::vector<std::shared_ptr<OrderItem>> items;
    OrderStatus status;
    PaymentStatus payment_status;
    std::shared_ptr<Delivery> delivery;
    bool is_free_delivery;
    double discount_amount;

    int pending_cashback; // <--- ÎÒËÎÆÅÍÍÛÉ ÊİØÁİÊ

public:
    Order(const std::string& id, std::shared_ptr<Customer> customer);

    std::string get_id() const;
    std::shared_ptr<Customer> get_customer() const;
    const std::vector<std::shared_ptr<OrderItem>>& get_items() const;
    OrderStatus get_status() const;
    PaymentStatus get_payment_status() const;
    DeliveryType get_delivery_type() const;

    void add_item(std::shared_ptr<OrderItem> item);
    void set_status(OrderStatus new_status);
    void set_payment_status(PaymentStatus new_status);
    void set_delivery(std::shared_ptr<Delivery> new_delivery);
    void set_free_delivery(bool free);

    double get_items_total() const;
    double get_total_price() const;
    double get_total_weight() const;
    int get_total_quantity() const;

    void add_discount_amount(double amount);
    double get_discount_amount() const;

    // Óïğàâëåíèå îòëîæåííûì êıøáıêîì
    void set_pending_cashback(int points);
    int get_pending_cashback() const;

    Order& operator+(const Discount& discount);
    friend std::ostream& operator<<(std::ostream& os, const Order& order);
};