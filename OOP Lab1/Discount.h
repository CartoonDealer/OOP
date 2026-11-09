#pragma once

// Предварительное объявление
class Order;

class Discount {
public:
    virtual ~Discount() = default;

    // Применение скидки к заказу (метод мутирует заказ)
    virtual void apply(Order& order) const = 0;
};