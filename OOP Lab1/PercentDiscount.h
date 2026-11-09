#pragma once
#include "Discount.h"

class PercentDiscount : public Discount {
private:
    int percent;
public:
    explicit PercentDiscount(int percent);
    void apply(Order& order) const override;
};