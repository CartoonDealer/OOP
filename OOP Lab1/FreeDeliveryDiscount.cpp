#include "FreeDeliveryDiscount.h"
#include "Order.h"

void FreeDeliveryDiscount::apply(Order& order) const {
    // Просто ставим флаг, Order сам обнулит доставку при подсчете
    order.set_free_delivery(true);
}