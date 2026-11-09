#include "PickupDelivery.h"
#include "Order.h"
#include "Customer.h"

double PickupDelivery::get_cost(const Order& order) const { return 0.0; }
std::string PickupDelivery::get_estimated_time() const { return "1-3 дня"; }

bool PickupDelivery::is_available(const Order& order) const {
    return get_unavailability_reason(order).empty();
}

std::string PickupDelivery::get_unavailability_reason(const Order& order) const {
    std::string region = order.get_customer()->get_region();
    if (region != "СПб" && region != "Санкт-Петербург") {
        return "Самовывоз недоступен. В вашем регионе (" + region + ") нет нашего ПВЗ.";
    }
    return "";
}

std::shared_ptr<Delivery> PickupDelivery::clone() const {
    return std::make_shared<PickupDelivery>(*this);
}