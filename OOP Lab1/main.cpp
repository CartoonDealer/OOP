#include <iostream>
#include <vector>
#include <memory>
#include <limits>
#include <string>
#include <stdexcept>
#include <clocale>
#include <cstdlib>
#include <iomanip>
#include <map>
#include <algorithm>

// Лаба 1: Склад
#include "TimeUtils.h"
#include "StandardFactory.h"
#include "FoodFactory.h"
#include "ChemicalFactory.h"
#include "Warehouse.h"
#include "PlacementManager.h"
#include "WarehouseInitializer.h"
#include "FoodProduct.h"

// Лаба 2: Заказы
#include "Customer.h"
#include "OrderManager.h"
#include "Order.h"
#include "PercentDiscount.h"
#include "FreeDeliveryDiscount.h"
#include "CourierDelivery.h"
#include "PickupDelivery.h"
#include "PostDelivery.h"
#include "DeliveryProxy.h"

// ==========================================
// Вспомогательные функции для ввода
// ==========================================
void clear_input() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int input_int(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            clear_input();
            return value;
        }
        else {
            std::cout << "Ошибка ввода. Введите число.\n";
            clear_input();
        }
    }
}

double input_double(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            clear_input();
            return value;
        }
        else {
            std::cout << "Ошибка ввода. Введите число.\n";
            clear_input();
        }
    }
}

std::string input_string(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    std::getline(std::cin, value);
    return value;
}

// ==========================================
// Функции вывода склада
// ==========================================
void print_cell(Cell& cell) {
    std::cout << "  Ячейка [" << cell.get_location_string() << "]";
    if (!cell.get_order_id().empty()) {
        std::cout << " | ЗАКАЗ: " << cell.get_order_id();
    }
    else {
        std::cout << " | Владелец: " << cell.get_seller_id();
    }
    std::cout << "\n";

    for (auto it = cell.begin(); it != cell.end(); ++it) {
        auto item = *it;
        std::cout << "    - [" << item->get_product()->get_id() << "] "
            << item->get_product()->get_name()
            << " (Кол-во: " << item->get_quantity() << ")\n";
    }
}

void print_warehouse(Warehouse& wh, const std::string& title) {
    std::cout << "\n--- " << title << " [" << wh.get_address() << "] ---\n";
    std::cout << "Вместимость: " << wh.get_occupied_cells_count() << " / " << wh.get_total_cells() << "\n";

    std::map<std::string, std::vector<Cell*>> stock_cells;
    std::map<std::string, std::vector<Cell*>> order_cells;

    for (auto it = wh.begin(); it != wh.end(); ++it) {
        if (!it->is_empty()) {
            if (it->get_order_id().empty()) {
                stock_cells[it->get_seller_id()].push_back(&(*it));
            }
            else {
                order_cells[it->get_order_id()].push_back(&(*it));
            }
        }
    }

    if (stock_cells.count("INTERNAL")) {
        std::cout << "\n>>> СЛУЖЕБНЫЕ ТОВАРЫ (INTERNAL)\n";
        for (auto* cell : stock_cells["INTERNAL"]) print_cell(*cell);
        stock_cells.erase("INTERNAL");
    }

    bool has_stock = false;
    for (auto const& pair : stock_cells) {
        has_stock = true;
        std::cout << "\n>>> СВОБОДНЫЙ СТОК: " << pair.first << "\n";
        for (auto* cell : pair.second) print_cell(*cell);
    }
    if (!has_stock && order_cells.empty()) {
        std::cout << "Склад пуст.\n";
    }

    if (!order_cells.empty()) {
        std::cout << "\n>>> СОБРАННЫЕ ЗАКАЗЫ (В ЯЧЕЙКАХ)\n";
        for (auto const& pair : order_cells) {
            std::cout << "  Заказ [" << pair.first << "]:\n";
            for (auto* cell : pair.second) print_cell(*cell);
        }
    }
}

// Поиск ТОЛЬКО СВОБОДНОГО товара
std::shared_ptr<OrderItem> find_free_item_in_warehouse(Warehouse& wh, const std::string& product_id) {
    for (auto it_wh = wh.begin(); it_wh != wh.end(); ++it_wh) {
        for (auto it_item = it_wh->begin(); it_item != it_wh->end(); ++it_item) {
            if ((*it_item)->get_order_id().empty() && (*it_item)->get_product()->get_id() == product_id) {
                return *it_item;
            }
        }
    }
    return nullptr;
}

// ==========================================
// УПРАВЛЕНИЕ ЗАКАЗАМИ (Активные / Архив)
// ==========================================
void process_orders_menu(std::vector<std::shared_ptr<Order>>& active_orders) {
    std::cout << "\n=== УПРАВЛЕНИЕ ЗАКАЗАМИ ===\n";
    std::cout << "1. Активные заказы (в работе)\n";
    std::cout << "2. Выполненные заказы (архив)\n";
    std::cout << "0. Назад\n";
    int sub_choice = input_int("Выбор: ");

    if (sub_choice == 0) return;

    std::vector<std::shared_ptr<Order>> display_orders;

    if (sub_choice == 1) {
        for (auto& o : active_orders) {
            if (o->get_status() != OrderStatus::ISSUED &&
                o->get_status() != OrderStatus::DELIVERED &&
                o->get_status() != OrderStatus::CANCELLED) {
                display_orders.push_back(o);
            }
        }
        if (display_orders.empty()) {
            std::cout << "Нет активных заказов в работе.\n";
            return;
        }
    }
    else if (sub_choice == 2) {
        for (auto& o : active_orders) {
            if (o->get_status() == OrderStatus::ISSUED ||
                o->get_status() == OrderStatus::DELIVERED ||
                o->get_status() == OrderStatus::CANCELLED) {
                display_orders.push_back(o);
            }
        }
        if (display_orders.empty()) {
            std::cout << "Архив пуст.\n";
            return;
        }
    }
    else return;

    for (size_t i = 0; i < display_orders.size(); ++i) {
        std::cout << i << ". Заказ " << display_orders[i]->get_id()
            << " | Доставка: " << status_to_string(display_orders[i]->get_status())
            << " | Оплата: " << payment_status_to_string(display_orders[i]->get_payment_status()) << "\n";
    }

    if (sub_choice == 2) {
        int idx = input_int("Выберите индекс заказа для просмотра (или -1 для выхода): ");
        if (idx >= 0 && idx < (int)display_orders.size()) {
            std::cout << "\n--- ДЕТАЛИ ЗАКАЗА ---\n";
            std::cout << *display_orders[idx];
        }
        return;
    }

    int idx = input_int("Выберите индекс заказа для обработки: ");
    if (idx < 0 || idx >= (int)display_orders.size()) return;

    auto order = display_orders[idx];
    DeliveryType d_type = order->get_delivery_type();

    std::cout << "\nТекущий статус доставки: " << status_to_string(order->get_status()) << "\n";

    try {
        if (d_type == DeliveryType::COURIER) {
            std::cout << "1. Сборка\n2. В пути\n3. Доставлено клиенту\n4. Отменить заказ\n";
            int s_choice = input_int("Новый статус: ");

            if (s_choice == 1) order->set_status(OrderStatus::PENDING);
            else if (s_choice == 2) order->set_status(OrderStatus::SHIPPED);
            else if (s_choice == 3) order->set_status(OrderStatus::DELIVERED);
            else if (s_choice == 4) order->set_status(OrderStatus::CANCELLED);

        }
        else if (d_type == DeliveryType::POST) {
            std::cout << "1. Сборка\n2. В пути\n3. Доставлено на почту\n4. Клиент забрал с почты\n5. Отменить заказ\n";
            int s_choice = input_int("Новый статус: ");

            if (s_choice == 1) order->set_status(OrderStatus::PENDING);
            else if (s_choice == 2) order->set_status(OrderStatus::SHIPPED);
            else if (s_choice == 3) order->set_status(OrderStatus::AT_POST_OFFICE);
            else if (s_choice == 4) order->set_status(OrderStatus::ISSUED);
            else if (s_choice == 5) order->set_status(OrderStatus::CANCELLED);

        }
        else { // PICKUP
            std::cout << "1. Сборка\n2. В пути (в ПВЗ)\n3. Привезено в ПВЗ\n4. Отменить заказ\n";
            int s_choice = input_int("Новый статус: ");

            if (s_choice == 1) order->set_status(OrderStatus::PENDING);
            else if (s_choice == 2) order->set_status(OrderStatus::SHIPPED);
            else if (s_choice == 3) order->set_status(OrderStatus::AT_PICKUP_POINT);
            else if (s_choice == 4) order->set_status(OrderStatus::CANCELLED);
        }

        std::cout << "Статус обновлен. Складской учет синхронизирован.\n";

    }
    catch (const std::exception& e) {
        std::cout << "[ОШИБКА]: " << e.what() << "\n";
    }
}

// ==========================================
// МАСТЕР ОФОРМЛЕНИЯ ЗАКАЗА
// ==========================================
void create_order_wizard(std::vector<std::shared_ptr<Customer>>& customers,
    std::vector<std::shared_ptr<Warehouse>>& main_warehouses,
    OrderManager& manager, std::vector<std::shared_ptr<Order>>& active_orders) {
    std::cout << "\n=== ОФОРМЛЕНИЕ НОВОГО ЗАКАЗА ===\n";

    std::cout << "Доступные клиенты:\n";
    for (size_t i = 0; i < customers.size(); ++i) {
        std::cout << i << ". " << customers[i]->get_full_name()
            << " (" << customers[i]->get_region() << ")"
            << " | Баланс: " << std::fixed << std::setprecision(2) << customers[i]->get_wallet_balance() << " руб."
            << " | Бонусы: " << std::defaultfloat << customers[i]->get_bonus_points() << "\n";
    }
    int c_idx = input_int("Выберите индекс клиента: ");
    if (c_idx < 0 || c_idx >= (int)customers.size()) throw std::out_of_range("Неверный индекс клиента");
    auto customer = customers[c_idx];

    int current_active_items = 0;
    for (const auto& o : active_orders) {
        if (o->get_customer()->get_id() == customer->get_id() &&
            o->get_status() != OrderStatus::ISSUED &&
            o->get_status() != OrderStatus::DELIVERED &&
            o->get_status() != OrderStatus::CANCELLED) {
            current_active_items += o->get_total_quantity();
        }
    }
    int available_slots = 15 - current_active_items;

    std::cout << "У клиента занято " << current_active_items << " из 15 товаров.\n";
    std::cout << "Доступно для добавления: " << available_slots << " шт.\n";

    if (available_slots <= 0) {
        std::cout << "Клиент исчерпал лимит в 15 товаров. Оформить заказ нельзя.\n";
        return;
    }

    std::cout << "Выберите склад, где будем собирать заказ:\n";
    for (size_t i = 0; i < main_warehouses.size(); ++i) {
        std::cout << i << ". [" << main_warehouses[i]->get_address() << "]\n";
    }
    int wh_idx = input_int("Индекс склада: ");
    if (wh_idx < 0 || wh_idx >= (int)main_warehouses.size()) throw std::out_of_range("Неверный индекс склада");

    std::vector<std::shared_ptr<OrderItem>> cart;
    std::map<std::string, int> reserved_qty;
    int cart_total_qty = 0;
    int item_num = 1;
    bool has_food = false;
    bool has_non_food = false;

    // СБОРКА КОРЗИНЫ
    while (true) {
        std::string p_id = input_string("Введите артикул товара (или '0' для завершения): ");
        if (p_id == "0") break;

        auto item = find_free_item_in_warehouse(*main_warehouses[wh_idx], p_id);
        if (!item) {
            std::cout << "Свободный товар с таким артикулом не найден.\n";
            continue;
        }

        // ПРОВЕРКА СМЕШИВАНИЯ
        bool is_food = (dynamic_cast<FoodProduct*>(item->get_product().get()) != nullptr);
        if (is_food && has_non_food) {
            std::cout << "[ОШИБКА] Нельзя смешивать продукты и обычные товары в одном заказе!\n";
            continue;
        }
        if (!is_food && has_food) {
            std::cout << "[ОШИБКА] В корзине уже есть продукты. Нельзя добавить обычный товар!\n";
            continue;
        }

        int already_reserved = reserved_qty[p_id];
        int free_qty = item->get_quantity() - already_reserved;

        std::cout << "Товар: " << item->get_product()->get_name()
            << " | Цена: " << item->get_product()->get_price().get_value()
            << " руб. | Доступно: " << free_qty << " шт.\n";

        int qty = input_int("Введите количество: ");
        if (qty <= 0 || qty > free_qty) {
            std::cout << "Неверное количество.\n";
            continue;
        }

        if (cart_total_qty + qty > available_slots) {
            std::cout << "[ОШИБКА] Превышен лимит! Вы можете добавить еще только "
                << (available_slots - cart_total_qty) << " шт.\n";
            continue;
        }

        auto new_item = std::make_shared<OrderItem>(item->get_seller_id(), "TEMP", item_num++, item->get_product(), qty);
        cart.push_back(new_item);
        cart_total_qty += qty;
        reserved_qty[p_id] += qty;

        if (is_food) has_food = true;
        else has_non_food = true;

        std::cout << "Товар добавлен в корзину.\n";
    }

    if (cart.empty()) {
        std::cout << "Корзина пуста. Заказ отменен.\n";
        return;
    }

    // ВЫБОР ДОСТАВКИ
    std::shared_ptr<Delivery> real_delivery;
    std::string delivery_name;
    bool delivery_selected = false;
    auto temp_order = std::make_shared<Order>("TEMP", customer);
    for (auto& ci : cart) temp_order->add_item(ci);

    if (has_food) {
        std::cout << "\n[ВНИМАНИЕ] В заказе есть продукты питания!\n";
        std::cout << "Из-за скоропортящихся товаров доступна только экспресс-доставка курьером.\n";

        auto courier = std::make_shared<CourierDelivery>();
        if (!courier->is_available(*temp_order)) {
            std::cout << "\n[ОШИБКА] Экспресс-доставка недоступна.\n";
            std::cout << "Причина: " << courier->get_unavailability_reason(*temp_order) << "\n";
            std::cout << "Мы не можем доставить продукты в ваш регион. Заказ отменен.\n";
            return;
        }
        real_delivery = courier;
        delivery_name = "Курьер (Экспресс)";
        delivery_selected = true;
    }
    else {
        while (!delivery_selected) {
            std::cout << "\nВыберите способ доставки:\n";
            std::cout << "1. Курьер (только СПб) (500 руб. до 5 кг + 50 руб за кг., В течение дня, макс. 20 кг)\n";
            std::cout << "2. ПВЗ (только СПб) (Бесплатно, 1-3 дня, без ограничений)\n";
            std::cout << "3. Почта (по всей РФ) (200 руб. до 5 кг + 30 руб за кг., 5-10 дней, макс. 10 кг)\n";
            int del_type = input_int("Выбор: ");

            if (del_type == 1) { real_delivery = std::make_shared<CourierDelivery>(); delivery_name = "Курьер"; }
            else if (del_type == 2) { real_delivery = std::make_shared<PickupDelivery>(); delivery_name = "ПВЗ"; }
            else if (del_type == 3) { real_delivery = std::make_shared<PostDelivery>(); delivery_name = "Почта"; }
            else {
                std::cout << "Неверный выбор.\n";
                continue;
            }

            if (!real_delivery->is_available(*temp_order)) {
                std::cout << "\n[ОШИБКА] Доставка недоступна.\n";
                std::cout << "Причина: " << real_delivery->get_unavailability_reason(*temp_order) << "\n";
                std::cout << "Пожалуйста, выберите другой способ доставки.\n";
            }
            else {
                delivery_selected = true;
            }
        }
    }

    std::shared_ptr<Delivery> proxy_delivery = std::make_shared<DeliveryProxy>(real_delivery);
    *proxy_delivery << *temp_order;

    bool used_percent = false;
    bool used_free_del = false;
    bool used_cashback = false;
    bool used_gift = false;

    // АКЦИИ
    while (true) {
        std::cout << "\n\\Клиент оформляет заказ, есть ли сейчас какие-то акции?\\\n";
        if (!used_percent) std::cout << "1. Процентная скидка 10%\n";
        if (!used_free_del) std::cout << "2. Бесплатная доставка\n";
        if (!used_cashback) std::cout << "3. Кэшбэк 5%\n";
        if (!used_gift) std::cout << "4. Добавить подарок\n";
        std::cout << "5. Завершить применение акций\n";

        if (used_percent && used_free_del && used_cashback && used_gift) {
            std::cout << "Все доступные акции применены.\n";
            break;
        }

        int promo = input_int("Выбор: ");

        if (promo == 1 && !used_percent) {
            *temp_order + PercentDiscount(10); used_percent = true;
            std::cout << "Скидка 10% применена.\n";
        }
        else if (promo == 2 && !used_free_del) {
            *temp_order + FreeDeliveryDiscount(); used_free_del = true;
            std::cout << "Бесплатная доставка применена.\n";
        }
        else if (promo == 3 && !used_cashback) {
            used_cashback = true;
            std::cout << "Кэшбэк 5% будет начислен после оплаты.\n";
        }
        else if (promo == 4 && !used_gift) {
            auto gift_item = find_free_item_in_warehouse(*main_warehouses[wh_idx], "800000001");
            int already_reserved = reserved_qty["800000001"];

            if (!gift_item || gift_item->get_quantity() - already_reserved <= 0) {
                std::cout << "Извините, подарки закончились!\n";
            }
            else if (cart_total_qty + 1 > available_slots) {
                std::cout << "[ОШИБКА] Лимит товаров не позволяет добавить подарок.\n";
            }
            else {
                auto order_gift = std::make_shared<OrderItem>(gift_item->get_seller_id(), "TEMP", 99, gift_item->get_product(), 1);
                cart.push_back(order_gift);
                temp_order->add_item(order_gift);
                cart_total_qty++;
                reserved_qty["800000001"] += 1;
                used_gift = true;
                std::cout << "Подарок добавлен в заказ.\n";
            }
        }
        else if (promo == 5) {
            break;
        }
        else {
            std::cout << "Неверный выбор или акция уже применена.\n";
        }
    }

    // ЧЕК
    double final_price = temp_order->get_total_price();
    double items_price_after_discount = temp_order->get_items_total() - temp_order->get_discount_amount();
    if (items_price_after_discount < 0) items_price_after_discount = 0;
    int cashback_points = 0;
    if (used_cashback) cashback_points = static_cast<int>(items_price_after_discount * 0.05);

    double delivery_cost = final_price - temp_order->get_items_total() + temp_order->get_discount_amount();
    if (delivery_cost < 0.0 && delivery_cost > -0.01) delivery_cost = 0.0;

    std::cout << "\n--- ЧЕК ---\n";
    for (auto& item : temp_order->get_items()) {
        std::cout << "  " << item->get_product()->get_name()
            << " | " << item->get_product()->get_price().get_value() << " руб. x "
            << item->get_quantity() << " = "
            << (item->get_product()->get_price().get_value() * item->get_quantity()) << " руб.\n";
    }
    std::cout << "-------------------\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Стоимость товаров: " << temp_order->get_items_total() << " руб.\n";
    std::cout << "Скидка: " << temp_order->get_discount_amount() << " руб.\n";
    std::cout << "Доставка (" << delivery_name << "): " << delivery_cost << " руб.\n";
    std::cout << "ИТОГО: " << final_price << " руб.\n";
    if (used_cashback) std::cout << "Начислится кэшбэк: " << cashback_points << " бонусов.\n";
    std::cout << std::defaultfloat;

    // ОПЛАТА
    bool is_paid = false;
    if (real_delivery->get_type() == DeliveryType::COURIER || real_delivery->get_type() == DeliveryType::POST) {
        std::cout << "\nТребуется предоплата.\n";
        int consent = input_int("Клиент, вы согласны списать " + std::to_string(int(final_price)) + " руб. с баланса? (1-да, 0-нет): ");
        if (consent == 1) {
            try {
                customer->deduct_funds(final_price);
                is_paid = true;
                std::cout << "Оплата прошла успешно!\n";
            }
            catch (const std::exception& e) {
                std::cout << "[ОШИБКА]: " << e.what() << " Заказ отменен.\n";
                return;
            }
        }
        else {
            std::cout << "Заказ отменен, так как клиент отказался от предоплаты.\n";
            return;
        }
    }
    else {
        std::cout << "\nВыберите способ оплаты в ПВЗ:\n";
        std::cout << "1. Оплатить сразу с баланса\n";
        std::cout << "2. Оплата при получении в ПВЗ\n";
        int pay_choice = input_int("Выбор: ");

        if (pay_choice == 1) {
            int consent = input_int("Клиент, вы согласны списать " + std::to_string(int(final_price)) + " руб. с баланса? (1-да, 0-нет): ");
            if (consent == 1) {
                try {
                    customer->deduct_funds(final_price);
                    is_paid = true;
                    std::cout << "Оплата прошла успешно!\n";
                }
                catch (const std::exception& e) {
                    std::cout << "[ОШИБКА]: " << e.what() << " Заказ отменен.\n";
                    return;
                }
            }
            else {
                std::cout << "Заказ отменен, так как клиент отказался от оплаты.\n";
                return;
            }
        }
        else {
            std::cout << "Заказ будет ожидать оплаты в ПВЗ.\n";
        }
    }

    // ТРАНЗАКЦИЯ: РАЗМЕЩЕНИЕ ТОВАРОВ С ОТКАТОМ
    auto final_order = manager.create_order(customer, cart);
    *proxy_delivery << *final_order;

    if (used_percent) *final_order + PercentDiscount(10);
    if (used_free_del) *final_order + FreeDeliveryDiscount();

    std::vector<std::shared_ptr<OrderItem>> placed_items;
    try {
        for (auto& order_item : final_order->get_items()) {
            PlacementManager::place_item(*main_warehouses[wh_idx], order_item, false);
            placed_items.push_back(order_item);
        }
    }
    catch (const std::exception& e) {
        std::cout << "\n[КРИТИЧЕСКАЯ ОШИБКА] Не удалось разместить товары на складе: " << e.what() << "\n";
        std::cout << "Заказ отменен. Возврат средств...\n";

        for (auto& p_item : placed_items) {
            auto cell = main_warehouses[wh_idx]->find_cell_with_item(p_item);
            if (cell) cell->remove_item(p_item);
        }
        if (is_paid) customer->add_funds(final_price);
        final_order->set_status(OrderStatus::CANCELLED);
        return;
    }

    // ТРАНЗАКЦИЯ: СПИСАНИЕ СВОБОДНОГО СТОКА С ОТКАТОМ
    bool stock_deducted = true;
    for (const auto& item : cart) {
        std::string p_id = item->get_product()->get_id();
        int qty_to_reduce = item->get_quantity();

        while (qty_to_reduce > 0) {
            auto free_item = find_free_item_in_warehouse(*main_warehouses[wh_idx], p_id);
            if (!free_item) {
                stock_deducted = false;
                break;
            }

            int to_take = std::min(qty_to_reduce, free_item->get_quantity());
            free_item->reduce_quantity(to_take);
            qty_to_reduce -= to_take;

            if (free_item->get_quantity() == 0) {
                auto cell = main_warehouses[wh_idx]->find_cell_with_item(free_item);
                if (cell) cell->remove_item(free_item);
            }
        }
        if (!stock_deducted) break;
    }

    if (!stock_deducted) {
        std::cout << "\n[КРИТИЧЕСКАЯ ОШИБКА] Товар закончился на складе во время оформления!\n";
        std::cout << "Заказ отменен. Возврат средств...\n";
        for (auto& p_item : placed_items) {
            auto cell = main_warehouses[wh_idx]->find_cell_with_item(p_item);
            if (cell) cell->remove_item(p_item);
        }
        if (is_paid) customer->add_funds(final_price);
        final_order->set_status(OrderStatus::CANCELLED);
        return;
    }

    // ВСЁ УСПЕШНО
    active_orders.push_back(final_order);

    if (is_paid) {
        final_order->set_payment_status(PaymentStatus::PAID);
        if (used_cashback) {
            customer->add_bonus_points(cashback_points);
            std::cout << "Бонусы (" << cashback_points << ") начислены сразу!\n";
        }
    }
    else {
        if (used_cashback) {
            final_order->set_pending_cashback(cashback_points);
            std::cout << "Кэшбэк (" << cashback_points << ") будет начислен после оплаты в ПВЗ.\n";
        }
    }

    std::cout << "Заказ успешно создан! Номер вашего заказа: " << final_order->get_id() << "\n";
}

// ==========================================
// МЕНЮ КОНКРЕТНОГО СКЛАДА
// ==========================================
void manage_warehouse(Warehouse& wh, const std::string& type_name, bool is_pvz,
    std::vector<std::shared_ptr<Order>>& active_orders,
    int& item_counter,
    StandardFactory& sf, FoodFactory& ff, ChemicalFactory& cf) {
    int choice;
    do {
        std::cout << "\n--- УПРАВЛЕНИЕ: " << type_name << " [" << wh.get_address() << "] ---\n";
        std::cout << "1. Показать товары\n";
        std::cout << "2. Добавить новый товар на склад\n"; // <--- ВОТ ОН, ВЕРНУЛИ!
        std::cout << "3. Удалить товар\n";
        std::cout << "4. Расширить склад\n";
        if (is_pvz) std::cout << "5. Выдать заказ\n";
        std::cout << "0. Назад\n";
        choice = input_int("Выбор: ");

        try {
            if (choice == 1) {
                print_warehouse(wh, type_name);
            }
            else if (choice == 2) {
                std::cout << "\n1. Обычный товар\n2. Еда\n3. Химия\n";
                int type = input_int("Тип товара: ");
                std::string manuf = input_string("Владелец (напр. ИП Сидоров): ");
                std::string name = input_string("Название: ");
                int price = input_int("Цена: ");
                int qty = input_int("Количество: ");
                double weight = input_double("Вес (кг): ");

                std::string p_id = "GEN-" + std::to_string(item_counter++);
                std::shared_ptr<Product> prod;
                bool is_express = false;

                if (type == 1) {
                    prod = sf.create_product(p_id, name, manuf, price, qty, weight);
                    is_express = (input_int("Требуется срочная доставка курьером? (1-да, 0-нет): ") == 1);
                }
                else if (type == 2) {
                    std::string d = input_string("Дата произв. (ДД.ММ.ГГГГ): ");
                    int life = input_int("Срок годности (дней): ");
                    prod = ff.create_product(p_id, name, manuf, price, qty, weight, Date(d), life);
                }
                else if (type == 3) {
                    std::string d = input_string("Дата произв. (ДД.ММ.ГГГГ): ");
                    int life = input_int("Срок годности (дней): ");
                    int tox = input_int("Токсично? (1-да, 0-нет): ");
                    prod = cf.create_product(p_id, name, manuf, price, qty, weight, Date(d), life, tox == 1);
                    is_express = (input_int("Требуется срочная доставка курьером? (1-да, 0-нет): ") == 1);
                }
                else {
                    std::cout << "Неверный тип.\n";
                    continue;
                }

                auto item = std::make_shared<OrderItem>(manuf, "", item_counter, prod, qty);
                PlacementManager::place_item(wh, item, is_express);
                std::cout << "Товар успешно добавлен на склад. ID: " << p_id << "\n";
            }
            else if (choice == 3) {
                std::string p_id = input_string("Введите артикул товара для удаления: ");
                std::shared_ptr<OrderItem> item = nullptr;
                for (auto it_wh = wh.begin(); it_wh != wh.end(); ++it_wh) {
                    for (auto it_item = it_wh->begin(); it_item != it_wh->end(); ++it_item) {
                        if ((*it_item)->get_product()->get_id() == p_id) { item = *it_item; break; }
                    }
                    if (item) break;
                }
                if (item) {
                    auto cell = wh.find_cell_with_item(item);
                    if (cell) cell->remove_item(item);
                    std::cout << "Товар удален.\n";
                }
                else std::cout << "Товар не найден.\n";
            }
            else if (choice == 4) {
                int count = input_int("На сколько ячеек расширить склад? ");
                wh.add_cells(count);
                std::cout << "Вместимость увеличена. Теперь ячеек: " << wh.get_total_cells() << "\n";
            }
            else if (choice == 5 && is_pvz) {
                std::cout << "\n=== ВЫДАЧА ЗАКАЗОВ В ПВЗ ===\n";
                std::string order_id = input_string("Введите номер заказа (ID): ");

                std::shared_ptr<Order> found_order = nullptr;
                for (auto& o : active_orders) {
                    if (o->get_id() == order_id) { found_order = o; break; }
                }

                if (!found_order) {
                    std::cout << "Заказ с таким номером не найден.\n";
                    continue;
                }
                if (found_order->get_status() != OrderStatus::AT_PICKUP_POINT) {
                    std::cout << "Заказ еще не привезен в ПВЗ.\n";
                    continue;
                }

                std::cout << "Статус оплаты: " << payment_status_to_string(found_order->get_payment_status()) << "\n";

                if (found_order->get_payment_status() == PaymentStatus::UNPAID) {
                    std::cout << "К оплате: " << std::fixed << std::setprecision(2) << found_order->get_total_price() << " руб.\n";
                    std::cout << "1. Списать со счета\n";
                    std::cout << "2. Оплата наличными\n";
                    std::cout << "3. Оплата картой\n";
                    std::cout << "4. Отмена\n";
                    int pay = input_int("Выбор: ");

                    if (pay == 1) {
                        auto customer = found_order->get_customer();
                        try {
                            customer->deduct_funds(found_order->get_total_price());
                            std::cout << "Оплата со счета прошла успешно!\n";
                            found_order->set_payment_status(PaymentStatus::PAID);
                        }
                        catch (const std::exception& e) {
                            std::cout << "[ОШИБКА]: " << e.what() << "\n";
                            std::cout << "Попробуйте выбрать другой способ оплаты:\n";
                            std::cout << "1. Оплата наличными\n";
                            std::cout << "2. Оплата картой\n";
                            std::cout << "3. Отмена\n";
                            int alt = input_int("Выбор: ");
                            if (alt == 1 || alt == 2) {
                                std::cout << "Оплата принята!\n";
                                found_order->set_payment_status(PaymentStatus::PAID);
                            }
                            else {
                                continue;
                            }
                        }
                    }
                    else if (pay == 2 || pay == 3) {
                        std::cout << "Оплата принята!\n";
                        found_order->set_payment_status(PaymentStatus::PAID);
                    }
                    else {
                        continue;
                    }
                }

                if (found_order->get_pending_cashback() > 0) {
                    found_order->get_customer()->add_bonus_points(found_order->get_pending_cashback());
                    std::cout << "Начислен отложенный кэшбэк: " << found_order->get_pending_cashback() << " бонусов.\n";
                    found_order->set_pending_cashback(0);
                }

                std::cout << "Выдаем заказ...\n";
                found_order->set_status(OrderStatus::ISSUED);
                std::cout << "Заказ успешно выдан и списан со склада!\n";
            }
        }
        catch (const std::exception& e) {
            std::cout << "\n[ОШИБКА]: " << e.what() << "\n";
        }
    } while (choice != 0);
}

// ==========================================
// ГЛАВНОЕ МЕНЮ
// ==========================================
int main() {
    setlocale(LC_ALL, "Russian");
    system("chcp 1251 > nul");

    StandardFactory standard_factory;
    FoodFactory food_factory;
    ChemicalFactory chemical_factory;
    Date today = TimeUtils::get_today();

    std::vector<std::shared_ptr<Warehouse>> main_warehouses;
    std::vector<std::shared_ptr<Warehouse>> pvz_warehouses;
    int item_counter = 1;

    auto base_main = std::make_shared<Warehouse>("г. Санкт-Петербург, ул. Складская, д. 15", 30);
    main_warehouses.push_back(base_main);
    auto base_pvz = std::make_shared<Warehouse>("г. Санкт-Петербург, ул. Ленина, д. 25", 10);
    pvz_warehouses.push_back(base_pvz);

    std::vector<std::string> manufacturers = { "ИП Иванов", "ИП Петров" };
    WarehouseInitializer::populate_stock(*base_main, standard_factory, food_factory, chemical_factory, today, manufacturers);
    WarehouseInitializer::populate_internal_main(*base_main, standard_factory);
    WarehouseInitializer::populate_internal_pvz(*base_pvz, standard_factory);

    OrderManager manager(15);
    manager.set_warehouses(main_warehouses, pvz_warehouses);

    std::vector<std::shared_ptr<Order>> active_orders;

    std::vector<std::shared_ptr<Customer>> customers;

    auto customer_spb = std::make_shared<Customer>("USER-001", "Иван", "Иванов", "СПб");
    customer_spb->add_funds(30000);
    customers.push_back(customer_spb);

    auto customer_msk = std::make_shared<Customer>("USER-002", "Петр", "Петров", "Москва");
    customer_msk->add_funds(30000);
    customers.push_back(customer_msk);

    int main_choice;
    do {
        std::cout << "\n============================================\n";
        std::cout << "          СИСТЕМА УПРАВЛЕНИЯ МАГАЗИНОМ\n";
        std::cout << "============================================\n";
        std::cout << "1. Посмотреть клиентов\n";
        std::cout << "2. Склады\n";
        std::cout << "3. Оформить новый заказ\n";
        std::cout << "4. Управление заказами\n";
        std::cout << "5. Статистика (OrderManager)\n";
        std::cout << "0. Выход\n";
        std::cout << "============================================\n";
        main_choice = input_int("Выберите действие: ");

        try {
            switch (main_choice) {
            case 1: {
                std::cout << "\n--- СПИСОК КЛИЕНТОВ ---\n";
                for (const auto& c : customers) {
                    std::cout << "ID: " << c->get_id() << " | " << c->get_full_name()
                        << " (" << c->get_region() << ")"
                        << " | Баланс: " << std::fixed << std::setprecision(2) << c->get_wallet_balance() << " руб."
                        << " | Бонусы: " << std::defaultfloat << c->get_bonus_points() << "\n";
                }
                break;
            }
            case 2: {
                int choice;
                do {
                    std::cout << "\n--- СКЛАДЫ ---\n";
                    std::cout << "1. Основные склады\n";
                    std::cout << "2. Пункты выдачи (ПВЗ)\n";
                    std::cout << "0. Назад\n";
                    choice = input_int("Выбор: ");

                    if (choice == 1) {
                        for (size_t i = 0; i < main_warehouses.size(); ++i) {
                            std::cout << i << ". [" << main_warehouses[i]->get_address() << "]\n";
                        }
                        int idx = input_int("Индекс склада: ");
                        if (idx >= 0 && idx < (int)main_warehouses.size()) {
                            manage_warehouse(*main_warehouses[idx], "Основной склад", false, active_orders, item_counter, standard_factory, food_factory, chemical_factory);
                        }
                    }
                    else if (choice == 2) {
                        for (size_t i = 0; i < pvz_warehouses.size(); ++i) {
                            std::cout << i << ". [" << pvz_warehouses[i]->get_address() << "]\n";
                        }
                        int idx = input_int("Индекс ПВЗ: ");
                        if (idx >= 0 && idx < (int)pvz_warehouses.size()) {
                            manage_warehouse(*pvz_warehouses[idx], "ПВЗ", true, active_orders, item_counter, standard_factory, food_factory, chemical_factory);
                        }
                    }
                } while (choice != 0);
                break;
            }
            case 3: {
                create_order_wizard(customers, main_warehouses, manager, active_orders);
                break;
            }
            case 4: {
                process_orders_menu(active_orders);
                break;
            }
            case 5: {
                manager.print_stats();
                break;
            }
            case 0:
                std::cout << "Выход...\n";
                break;
            default:
                std::cout << "Неверный пункт.\n";
                break;
            }
        }
        catch (const std::exception& e) {
            std::cout << "\n[ОШИБКА]: " << e.what() << "\n";
        }

    } while (main_choice != 0);

    return 0;
}