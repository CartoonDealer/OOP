#pragma once
#include "Warehouse.h"
#include "StandardFactory.h"
#include "FoodFactory.h"
#include "ChemicalFactory.h"
#include "Date.h"
#include <string>
#include <vector>

class WarehouseInitializer {
public:
    // Генерация реалистичных товаров от продавцов
    static void populate_stock(Warehouse& wh, StandardFactory& sf, FoodFactory& ff, ChemicalFactory& cf,
        const Date& today, const std::vector<std::string>& manufacturers);

    // Внутренний инвентарь для ОСНОВНОГО СКЛАДА
    static void populate_internal_main(Warehouse& wh, StandardFactory& sf);

    // Внутренний инвентарь для ПВЗ
    static void populate_internal_pvz(Warehouse& wh, StandardFactory& sf);
};