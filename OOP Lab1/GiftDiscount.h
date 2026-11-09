#pragma once
#include "Discount.h"
#include "OrderItem.h"
#include <memory>

class GiftDiscount : public Discount {
private:
    std::shared_ptr<OrderItem> gift_item; // Брошюра, которую мы дарим
public:
    explicit GiftDiscount(std::shared_ptr<OrderItem> gift);
    void apply(Order& order) const override;
};