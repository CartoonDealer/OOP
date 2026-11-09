#pragma once
#include <vector>
#include <memory>
#include <string>
#include <map>
#include <set>
#include "Order.h"
#include "Customer.h"
#include "OrderItem.h"
#include "IObserver.h"
#include "Warehouse.h"

class OrderManager : public IObserver {
private:
    std::vector<std::shared_ptr<Order>> orders;
    std::set<std::string> unique_customers;
    double turnover;

    int max_active_orders;
    int next_order_id;

    std::vector<std::shared_ptr<Warehouse>> main_warehouses;
    std::vector<std::shared_ptr<Warehouse>> pvz_warehouses;

    void sync_inventory(const std::shared_ptr<Order>& order);

public:
    explicit OrderManager(int max_orders = 15);

    void set_warehouses(const std::vector<std::shared_ptr<Warehouse>>& main_whs,
        const std::vector<std::shared_ptr<Warehouse>>& pvz_whs);

    std::shared_ptr<Order> create_order(std::shared_ptr<Customer> customer,
        const std::vector<std::shared_ptr<OrderItem>>& items);

    void on_order_event(const OrderEvent& event) override;

    // Статистика
    double get_turnover() const;
    int get_customer_count() const;
    int get_active_orders_count() const;
    int get_completed_orders_count() const; 
    int get_cancelled_orders_count() const; 
    int get_pvz_orders_count() const;
    void print_stats() const;

    const std::vector<std::shared_ptr<Order>>& get_orders() const;
};