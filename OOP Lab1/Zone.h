#pragma once
#include <string>

enum class Zone {
    SHELF,         // Стеллаж (S)
    BIG,           // Крупногабарит (B)
    RET,           // Возвраты (R)
    DELIVERY_FOOD, // Доставка Еда (DF)
    DELIVERY_OTHER,// Доставка Остальное (DO)
    INTERNAL       // Внутренние нужды (I) <--- НОВАЯ ЗОНА
};

inline std::string get_zone_prefix(Zone zone) {
    switch (zone) {
    case Zone::SHELF: return "S";
    case Zone::BIG: return "B";
    case Zone::RET: return "R";
    case Zone::DELIVERY_FOOD: return "DF";
    case Zone::DELIVERY_OTHER: return "DO";
    case Zone::INTERNAL: return "I"; // <--- Префикс
    default: return "UNKNOWN";
    }
}