#pragma once
#include <vector>
#include <algorithm> // Для std::remove
#include "IObserver.h"

class IObservable {
protected:
    std::vector<IObserver*> observers;

public:
    virtual ~IObservable() = default;

    void subscribe(IObserver* observer) {
        // Защита от дублирования (один и тот же наблюдатель не должен подписываться дважды)
        if (std::find(observers.begin(), observers.end(), observer) == observers.end()) {
            observers.push_back(observer);
        }
    }

    void unsubscribe(IObserver* observer) {
        // Идиома Erase-Remove: сдвигаем нужный элемент в конец и удаляем
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

    void notify(const OrderEvent& event) {
        for (auto* observer : observers) {
            observer->on_order_event(event);
        }
    }
};