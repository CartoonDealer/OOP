#include "PercentDiscount.h"
#include "Order.h"

PercentDiscount::PercentDiscount(int percent) : percent(percent) {}

void PercentDiscount::apply(Order& order) const {
    // —читаем скидку от суммы товаров (без учета доставки)
    double items_total = order.get_items_total();
    double discount = items_total * (percent / 100.0);
    order.add_discount_amount(discount);
}