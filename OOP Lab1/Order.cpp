#include "Order.h"

Order::Order(const std::string& id, std::shared_ptr<Customer> customer)
    : id(id), customer(customer), status(OrderStatus::PENDING),
    payment_status(PaymentStatus::UNPAID), is_free_delivery(false),
    discount_amount(0.0), pending_cashback(0) {
}

std::string Order::get_id() const { return id; }
std::shared_ptr<Customer> Order::get_customer() const { return customer; }
const std::vector<std::shared_ptr<OrderItem>>& Order::get_items() const { return items; }
OrderStatus Order::get_status() const { return status; }
PaymentStatus Order::get_payment_status() const { return payment_status; }

DeliveryType Order::get_delivery_type() const {
    if (delivery == nullptr) return DeliveryType::PICKUP;
    return delivery->get_type();
}

void Order::add_item(std::shared_ptr<OrderItem> item) { items.push_back(item); }

void Order::set_status(OrderStatus new_status) {
    if (status == new_status) return;

    OrderStatus old = status;
    status = new_status;

    OrderEvent event(id, old, status, payment_status, payment_status);

    try {
        notify(event); // Пытаемся оповестить (и переместить товары)
    }
    catch (const std::exception& e) {
        status = old; // ОТКАТ: Если не удалось переместить, возвращаем старый статус
        std::cout << "[ORDER] Ошибка смены статуса. Статус возвращен на " << status_to_string(status) << "\n";
        throw std::runtime_error("Не удалось обновить статус заказа: " + std::string(e.what()));
    }
}

void Order::set_payment_status(PaymentStatus new_status) {
    if (payment_status == new_status) return;
    PaymentStatus old = payment_status;
    payment_status = new_status;
    OrderEvent event(id, status, status, old, payment_status);
    notify(event);
}

void Order::set_delivery(std::shared_ptr<Delivery> new_delivery) { delivery = new_delivery; }
void Order::set_free_delivery(bool free) { is_free_delivery = free; }

void Order::add_discount_amount(double amount) { if (amount > 0) discount_amount += amount; }
double Order::get_discount_amount() const { return discount_amount; }

void Order::set_pending_cashback(int points) { pending_cashback = points; }
int Order::get_pending_cashback() const { return pending_cashback; }

double Order::get_items_total() const {
    double items_total = 0.0;
    for (const auto& item : items) items_total += item->get_product()->get_price().get_value() * item->get_quantity();
    return items_total;
}

double Order::get_total_price() const {
    double final_items_total = get_items_total() - discount_amount;
    if (final_items_total < 0) final_items_total = 0;

    double delivery_cost = 0.0;
    if (delivery != nullptr && !is_free_delivery) delivery_cost = delivery->get_cost(*this);
    return final_items_total + delivery_cost;
}

double Order::get_total_weight() const {
    double total = 0.0;
    for (const auto& item : items) total += item->get_product()->get_weight().get_value() * item->get_quantity();
    return total;
}

int Order::get_total_quantity() const {
    int total = 0;
    for (const auto& item : items) total += item->get_quantity();
    return total;
}

Order& Order::operator+(const Discount& discount) {
    discount.apply(*this);
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Order& order) {
    os << "=== Заказ #" << order.id << " ===\n";
    os << "Клиент: " << order.customer->get_full_name() << "\n";
    os << "Статус доставки: " << status_to_string(order.status) << "\n";
    os << "Статус оплаты: " << payment_status_to_string(order.payment_status) << "\n";
    os << "Товары:\n";
    for (const auto& item : order.items) {
        os << "  - " << item->get_product()->get_name() << " x" << item->get_quantity()
            << " (Локация: " << item->get_location() << ")\n";
    }
    if (order.discount_amount > 0) os << "Скидка: -" << order.discount_amount << " руб.\n";
    os << "Итого к оплате: " << order.get_total_price() << " руб.\n";
    return os;
}

Order& operator<<(const Delivery& delivery, Order& order) {
    order.set_delivery(delivery.clone());
    return order;
}