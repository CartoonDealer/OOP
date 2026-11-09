#include "CashbackDiscount.h"
#include "Order.h"

CashbackDiscount::CashbackDiscount(int percent) : percent(percent) {}

void CashbackDiscount::apply(Order& order) const {
    // Начисляем баллы клиенту от суммы товаров
    double items_total = order.get_items_total();
    int points = static_cast<int>(items_total * (percent / 100.0));
    order.get_customer()->add_bonus_points(points);
}