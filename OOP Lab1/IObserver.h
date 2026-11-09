#pragma once
#include "OrderEvent.h"

class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void on_order_event(const OrderEvent& event) = 0;
};