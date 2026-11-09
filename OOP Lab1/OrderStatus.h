#pragma once
#include <string>

enum class OrderStatus {
    PENDING,        // Ожидает сборки
    SHIPPED,        // В пути
    AT_PICKUP_POINT,// Ожидает в ПВЗ
    AT_POST_OFFICE, // Ожидает на почте
    DELIVERED,      // Доставлено клиенту (для курьера)
    ISSUED,         // Выдан (успешное завершение)
    RETURNING,      // Возврат
    CANCELLED       // Отменен
};

enum class PaymentStatus {
    UNPAID,         // Ожидает оплаты
    PAID,           // Оплачен
    REFUNDED        // Возврат средств
};

inline std::string status_to_string(OrderStatus status) {
    switch (status) {
    case OrderStatus::PENDING: return "PENDING (Сборка)";
    case OrderStatus::SHIPPED: return "SHIPPED (В пути)";
    case OrderStatus::AT_PICKUP_POINT: return "AT_PICKUP_POINT (В ПВЗ)";
    case OrderStatus::AT_POST_OFFICE: return "AT_POST_OFFICE (На почте)";
    case OrderStatus::DELIVERED: return "DELIVERED (Доставлено клиенту)";
    case OrderStatus::ISSUED: return "ISSUED (Выдан)";
    case OrderStatus::RETURNING: return "RETURNING (Возврат)";
    case OrderStatus::CANCELLED: return "CANCELLED (Отменен)";
    default: return "UNKNOWN";
    }
}

inline std::string payment_status_to_string(PaymentStatus status) {
    switch (status) {
    case PaymentStatus::UNPAID: return "ОЖИДАЕТ ОПЛАТЫ";
    case PaymentStatus::PAID: return "ОПЛАЧЕН";
    case PaymentStatus::REFUNDED: return "ВОЗВРАТ СРЕДСТВ";
    default: return "НЕИЗВЕСТНО";
    }
}