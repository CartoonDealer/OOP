#include "WarehouseInitializer.h"
#include "PlacementManager.h"

void WarehouseInitializer::populate_stock(Warehouse& wh, StandardFactory& sf, FoodFactory& ff, ChemicalFactory& cf,
    const Date& today, const std::vector<std::string>& manufacturers) {

    std::string seller_ivanov = "ИП Иванов";
    std::string seller_petrov = "ИП Петров";
    std::string seller_food = "АО Молоко";
    std::string seller_gift = "Магазин";

    // --- ТОВАРЫ ИП ИВАНОВ (ХИМИЯ) ---
    auto chem1 = cf.create_product("100000001", "Мыло Dove", seller_ivanov, 150, 50, 0.1, today, 365, false);
    PlacementManager::place_item(wh, std::make_shared<OrderItem>(seller_ivanov, "", 1, chem1, 50), false);

    auto chem2 = cf.create_product("100000002", "Шампунь Head&Shoulders", seller_ivanov, 450, 30, 0.4, today, 365, false);
    PlacementManager::place_item(wh, std::make_shared<OrderItem>(seller_ivanov, "", 2, chem2, 30), false);

    auto chem3 = cf.create_product("100000003", "Порошок Tide 3кг", seller_ivanov, 1200, 20, 3.0, today, 365, true);
    PlacementManager::place_item(wh, std::make_shared<OrderItem>(seller_ivanov, "", 3, chem3, 20), false);

    auto chem4 = cf.create_product("100000004", "Средство для посуды Fairy", seller_ivanov, 250, 40, 0.5, today, 365, false);
    PlacementManager::place_item(wh, std::make_shared<OrderItem>(seller_ivanov, "", 4, chem4, 40), false);

    auto chem5 = cf.create_product("100000005", "Отбеливатель Белизна", seller_ivanov, 100, 15, 1.0, today, 365, true);
    PlacementManager::place_item(wh, std::make_shared<OrderItem>(seller_ivanov, "", 5, chem5, 15), false);


    // --- ТОВАРЫ ИП ПЕТРОВ (ЭЛЕКТРОНИКА) ---
    auto tech1 = sf.create_product("200000001", "Смартфон iPhone 15", seller_petrov, 90000, 5, 0.2);
    PlacementManager::place_item(wh, std::make_shared<OrderItem>(seller_petrov, "", 6, tech1, 5), false);

    auto tech2 = sf.create_product("200000002", "Ноутбук ASUS ROG", seller_petrov, 75000, 3, 2.5);
    PlacementManager::place_item(wh, std::make_shared<OrderItem>(seller_petrov, "", 7, tech2, 3), false);

    auto tech3 = sf.create_product("200000003", "Наушники Sony", seller_petrov, 15000, 10, 0.3);
    PlacementManager::place_item(wh, std::make_shared<OrderItem>(seller_petrov, "", 8, tech3, 10), false);

    auto tech4 = sf.create_product("200000004", "Телевизор LG 55\"", seller_petrov, 60000, 2, 15.0);
    PlacementManager::place_item(wh, std::make_shared<OrderItem>(seller_petrov, "", 9, tech4, 2), false);

    auto tech5 = sf.create_product("200000005", "Мышь Logitech", seller_petrov, 3000, 20, 0.1);
    PlacementManager::place_item(wh, std::make_shared<OrderItem>(seller_petrov, "", 10, tech5, 20), false);


    // --- ЕДА (АО МОЛОКО) ---
    auto food1 = ff.create_product("300000001", "Молоко Простоквашино", seller_food, 85, 20, 1.0, today, 7);
    PlacementManager::place_item(wh, std::make_shared<OrderItem>(seller_food, "", 11, food1, 20), false);

    auto food2 = ff.create_product("300000002", "Хлеб Бородинский", seller_food, 45, 15, 0.4, today, 3);
    PlacementManager::place_item(wh, std::make_shared<OrderItem>(seller_food, "", 12, food2, 15), false);

    // --- ПОДАРКИ (ВЛАДЕЛЕЦ - МАГАЗИН) ---
    auto gift = sf.create_product("800000001", "Подарок", seller_gift, 0, 10, 0.01);
    PlacementManager::place_item(wh, std::make_shared<OrderItem>(seller_gift, "", 13, gift, 10), false);
}

void WarehouseInitializer::populate_internal_main(Warehouse& wh, StandardFactory& sf) {
    std::string seller = "INTERNAL";
    std::string manuf = "INTERNAL";

    auto scissors = sf.create_product("900000001", "Ножницы", manuf, 0, 50, 0.1);
    PlacementManager::place_item(wh, std::make_shared<OrderItem>(seller, "", 1, scissors, 50), false);

    auto tape = sf.create_product("900000002", "Скотч", manuf, 0, 50, 0.2);
    PlacementManager::place_item(wh, std::make_shared<OrderItem>(seller, "", 2, tape, 50), false);

    auto bags = sf.create_product("900000003", "Пакеты", manuf, 0, 10000, 2.0);
    PlacementManager::place_item(wh, std::make_shared<OrderItem>(seller, "", 3, bags, 10000), false);
}

void WarehouseInitializer::populate_internal_pvz(Warehouse& wh, StandardFactory& sf) {
    std::string seller = "INTERNAL";
    std::string manuf = "INTERNAL";

    auto pc = sf.create_product("900000004", "Компьютер кассира", manuf, 0, 1, 5.0);
    PlacementManager::place_item(wh, std::make_shared<OrderItem>(seller, "", 1, pc, 1), false);

    auto tape = sf.create_product("900000002", "Скотч", manuf, 0, 2, 0.2);
    PlacementManager::place_item(wh, std::make_shared<OrderItem>(seller, "", 2, tape, 2), false);

    auto scissors = sf.create_product("900000001", "Ножницы", manuf, 0, 3, 0.1);
    PlacementManager::place_item(wh, std::make_shared<OrderItem>(seller, "", 3, scissors, 3), false);

    auto bags = sf.create_product("900000003", "Пакеты", manuf, 0, 100, 1.0);
    PlacementManager::place_item(wh, std::make_shared<OrderItem>(seller, "", 4, bags, 100), false);
}