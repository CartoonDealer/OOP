#include "OrderManager.h"
#include "PlacementManager.h"
#include <stdexcept>
#include <iostream>
#include <iomanip>

OrderManager::OrderManager(int max_orders)
    : turnover(0.0), max_active_orders(max_orders), next_order_id(1) {
}

void OrderManager::set_warehouses(const std::vector<std::shared_ptr<Warehouse>>& main_whs,
    const std::vector<std::shared_ptr<Warehouse>>& pvz_whs) {
    main_warehouses = main_whs;
    pvz_warehouses = pvz_whs;
}

std::shared_ptr<Order> OrderManager::create_order(std::shared_ptr<Customer> customer,
    const std::vector<std::shared_ptr<OrderItem>>& items) {
    int current_items_count = 0;
    int active_orders_count = 0;

    for (const auto& order : orders) {
        // Заказ считается активным, если он не выдан, не доставлен и не отменен
        if (order->get_customer()->get_id() == customer->get_id() &&
            order->get_status() != OrderStatus::ISSUED &&
            order->get_status() != OrderStatus::DELIVERED &&
            order->get_status() != OrderStatus::CANCELLED) {
            active_orders_count++;
            current_items_count += order->get_total_quantity();
        }
    }

    int new_order_quantity = 0;
    for (const auto& item : items) new_order_quantity += item->get_quantity();

    if (current_items_count + new_order_quantity > 15) {
        throw std::runtime_error("Нельзя заказать больше 15 товаров суммарно");
    }
    if (active_orders_count >= max_active_orders) {
        throw std::runtime_error("Достигнут лимит активных заказов");
    }

    std::string order_id = "ORD-" + std::to_string(next_order_id++);
    auto new_order = std::make_shared<Order>(order_id, customer);

    for (const auto& item : items) {
        item->set_order_id(order_id);
        new_order->add_item(item);
    }

    new_order->subscribe(this);
    orders.push_back(new_order);
    unique_customers.insert(customer->get_id());

    return new_order;
}

void OrderManager::on_order_event(const OrderEvent& event) {
    if (event.get_new_payment() == PaymentStatus::PAID && event.get_old_payment() != PaymentStatus::PAID) {
        for (const auto& order : orders) {
            if (order->get_id() == event.get_order_id()) {
                turnover += order->get_total_price();
                std::cout << "[Менеджер] Заказ " << event.get_order_id() << " ОПЛАЧЕН. Оборот: "
                    << std::fixed << std::setprecision(2) << turnover << " руб.\n";
                break;
            }
        }
    }

    if (event.get_old_status() != event.get_new_status()) {
        std::cout << "[Менеджер] Заказ " << event.get_order_id()
            << " сменил статус на " << status_to_string(event.get_new_status()) << "\n";

        for (const auto& order : orders) {
            if (order->get_id() == event.get_order_id()) {
                sync_inventory(order);
                break;
            }
        }
    }
}

void OrderManager::sync_inventory(const std::shared_ptr<Order>& order) {
    if (!order) return;
    std::string order_id = order->get_id();
    OrderStatus status = order->get_status();

    // 1. Определяем, куда физически должен лечь заказ
    std::shared_ptr<Warehouse> target_wh = nullptr;
    if (status == OrderStatus::PENDING && !main_warehouses.empty()) target_wh = main_warehouses[0];
    else if ((status == OrderStatus::AT_PICKUP_POINT || status == OrderStatus::AT_POST_OFFICE) && !pvz_warehouses.empty()) target_wh = pvz_warehouses[0];

    // 2. Пытаемся разложить на новом месте
    if (target_wh) {
        for (auto& item : order->get_items()) {
            if (target_wh->find_cell_with_item(item) == nullptr) {
                try {
                    PlacementManager::place_item(*target_wh, item, false);
                }
                catch (const std::exception& e) {
                    // ОТКАТ: Если склад переполнен, убираем то, что успели положить
                    target_wh->remove_order_items(order_id);
                    throw std::runtime_error("Целевой склад переполнен или недоступен: " + std::string(e.what()));
                }
            }
        }
    }

    // 3. Если всё разложилось успешно, ТОЛЬКО ТЕПЕРЬ удаляем со старых складов
    for (auto& wh : main_warehouses) if (wh != target_wh) wh->remove_order_items(order_id);
    for (auto& wh : pvz_warehouses) if (wh != target_wh) wh->remove_order_items(order_id);
}

double OrderManager::get_turnover() const { return turnover; }
int OrderManager::get_customer_count() const { return unique_customers.size(); }

int OrderManager::get_active_orders_count() const {
    int count = 0;
    for (const auto& o : orders) {
        if (o->get_status() != OrderStatus::ISSUED &&
            o->get_status() != OrderStatus::DELIVERED &&
            o->get_status() != OrderStatus::CANCELLED) {
            count++;
        }
    }
    return count;
}

// НОВЫЙ МЕТОД: считает выполненные (доставленные или выданные)
int OrderManager::get_completed_orders_count() const {
    int count = 0;
    for (const auto& o : orders) {
        if (o->get_status() == OrderStatus::ISSUED || o->get_status() == OrderStatus::DELIVERED) {
            count++;
        }
    }
    return count;
}

// НОВЫЙ МЕТОД: считает отмененные
int OrderManager::get_cancelled_orders_count() const {
    int count = 0;
    for (const auto& o : orders) {
        if (o->get_status() == OrderStatus::CANCELLED) {
            count++;
        }
    }
    return count;
}

int OrderManager::get_pvz_orders_count() const {
    int count = 0;
    for (const auto& o : orders) {
        if (o->get_status() == OrderStatus::AT_PICKUP_POINT || o->get_status() == OrderStatus::AT_POST_OFFICE) {
            count++;
        }
    }
    return count;
}

const std::vector<std::shared_ptr<Order>>& OrderManager::get_orders() const {
    return orders;
}

void OrderManager::print_stats() const {
    std::cout << "\n--- СТАТИСТИКА МАГАЗИНА ---\n";
    std::cout << "Всего заказов оформлено: " << orders.size() << "\n";
    std::cout << "Уникальных клиентов: " << unique_customers.size() << "\n";
    std::cout << "Активных заказов (в работе): " << get_active_orders_count() << "\n";
    std::cout << "Выполненных заказов (доставлено/выдано): " << get_completed_orders_count() << "\n"; 
    std::cout << "Отмененных заказов: " << get_cancelled_orders_count() << "\n"; 
    std::cout << "Заказов ожидают в ПВЗ/Почте: " << get_pvz_orders_count() << "\n";
    std::cout << "Общий оборот (оплачено): " << std::fixed << std::setprecision(2) << turnover << " руб.\n";
    std::cout << "---------------------------\n";
}