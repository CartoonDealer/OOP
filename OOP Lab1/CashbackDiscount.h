#pragma once
#include "Discount.h"

class CashbackDiscount : public Discount {
private:
    int percent;
public:
    explicit CashbackDiscount(int percent);
    void apply(Order& order) const override;
};