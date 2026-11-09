#include "GiftDiscount.h"
#include "Order.h"

GiftDiscount::GiftDiscount(std::shared_ptr<OrderItem> gift) : gift_item(gift) {}

void GiftDiscount::apply(Order& order) const {
    // Просто добавляем брошюру в заказ
    order.add_item(gift_item);
}