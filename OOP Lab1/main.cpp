#include <iostream>
#include <memory>
#include <vector>
#include <limits>
#include <string>
#include <stdexcept>
#include <clocale>
#include <cstdlib>
#include <iomanip>

#include "ProductTypes.h"
#include "Factories.h"
#include "Warehouse.h"

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
            std::cout << "Ошибка ввода. Введите целое число.\n";
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

void print_warehouse(const std::string& title, Warehouse& wh) {
    std::cout << "\n--- " << title << " (Товаров: " << wh.size() << " / " << wh.capacity() << ") ---\n";
    if (wh.size() == 0) {
        std::cout << "Склад пуст.\n";
        return;
    }

    size_t index = 0;
    for (auto it = wh.begin(); it != wh.end(); ++it) {
        std::cout << "[" << index << "] "; 
        it->print(std::cout);
        std::cout << "\n";
        index++;
    }
}

Warehouse* select_warehouse(Warehouse* main_wh, Warehouse* second_wh) {
    std::cout << "Выберите склад:\n1. Основной склад\n2. Второй склад\n";
    int choice = input_int("Ваш выбор: ");
    if (choice == 1) return main_wh;
    if (choice == 2) return second_wh;
    std::cout << "Неверный выбор. Действие отменено.\n";
    return nullptr;
}

int main() {
    setlocale(LC_ALL, "Russian");
    system("chcp 1251 > nul");

    std::unique_ptr<Warehouse> main_wh = std::make_unique<Warehouse>(10);
    std::unique_ptr<Warehouse> second_wh = std::make_unique<Warehouse>(5);

    SmartphoneFactory sf;
    LaptopFactory lf;
    TShirtFactory tf;
    JacketFactory jf;
    MilkFactory mf;
    BreadFactory bf;

    int choice;
    do {
        std::cout << "\n============================================\n";
        std::cout << "       СИСТЕМА УПРАВЛЕНИЯ СКЛАДОМ\n";
        std::cout << "============================================\n";
        std::cout << "1. Создать новый основной склад\n";
        std::cout << "2. Добавить товар на основной склад\n";
        std::cout << "3. Вывести основной склад\n";
        std::cout << "4. Вывести второй склад\n";
        std::cout << "5. Удалить товар по ID\n";
        std::cout << "6. Удалить товар по индексу\n";
        std::cout << "7. Поиск по ID\n";
        std::cout << "8. Поиск по категории\n";
        std::cout << "9. Поиск по цене\n";
        std::cout << "10. Изменить цену товара\n";
        std::cout << "11. Изменить количество товара\n";
        std::cout << "12. Переместить товар с основного на второй склад\n";
        std::cout << "0. Выход\n";
        std::cout << "============================================\n";

        choice = input_int("Выберите действие: ");

        try {
            switch (choice) {
            case 1: {
                int cap = input_int("Введите максимальную вместимость: ");
                main_wh = std::make_unique<Warehouse>(cap);
                std::cout << "Создан новый склад на " << cap << " мест.\n";
                break;
            }
            case 2: {
                std::cout << "\n1. Смартфон\n2. Ноутбук\n3. Футболка\n4. Куртка\n5. Молоко\n6. Хлеб\n";
                int type = input_int("Выберите тип товара: ");

                std::string name = input_string("Введите название: ");
                std::string id = Product::generate_id();
                int price = input_int("Введите цену (руб.): ");
                int qty = input_int("Введите количество: ");
                double weight = input_double("Введите вес (г): ");

                std::unique_ptr<Product> prod;
                if (type == 1) prod = sf.create_product(id, name, price, qty, weight);
                else if (type == 2) prod = lf.create_product(id, name, price, qty, weight);
                else if (type == 3) prod = tf.create_product(id, name, price, qty, weight);
                else if (type == 4) prod = jf.create_product(id, name, price, qty, weight);
                else if (type == 5 || type == 6) {
                    std::string date = input_string("Введите дату производства (ДД.ММ.ГГГГ): ");
                    int shelf = input_int("Введите срок годности (дней): ");
                    if (type == 5) prod = mf.create_product(id, name, price, qty, weight, date, shelf);
                    else prod = bf.create_product(id, name, price, qty, weight, date, shelf);
                }
                else {
                    std::cout << "Неверный тип.\n";
                    break;
                }

                main_wh->add_product(std::move(prod));
                std::cout << "Товар добавлен. ID: " << id << "\n";
                break;
            }
            case 3: print_warehouse("Основной склад", *main_wh); break;
            case 4: print_warehouse("Второй склад", *second_wh); break;
            case 5: {
                Warehouse* wh = select_warehouse(main_wh.get(), second_wh.get());
                if (!wh) break;

                std::string id = input_string("Введите ID товара для удаления: ");
                wh->remove_by_id(id);
                std::cout << "Товар удален.\n";
                break;
            }
            case 6: {
                Warehouse* wh = select_warehouse(main_wh.get(), second_wh.get());
                if (!wh) break;

                int idx = input_int("Введите индекс товара для удаления: ");
                wh->remove_by_index(idx);
                std::cout << "Товар удален.\n";
                break;
            }
            case 7: {
                std::string id = input_string("Введите ID для поиска: ");
                Product* p1 = main_wh->find_by_id(id);
                Product* p2 = second_wh->find_by_id(id);

                if (p1) { std::cout << "[Основной склад] "; p1->print(std::cout); std::cout << "\n"; }
                else if (p2) { std::cout << "[Второй склад] "; p2->print(std::cout); std::cout << "\n"; }
                else std::cout << "Товар с таким ID не найден ни на одном складе.\n";
                break;
            }
            case 8: {
                std::string cat = input_string("Введите категорию (electronics, clothing, food): ");
                auto res1 = main_wh->find_by_category(cat);
                auto res2 = second_wh->find_by_category(cat);

                std::cout << "\n--- Результаты поиска ---\n";
                std::cout << "На основном складе найдено: " << res1.size() << "\n";
                for (auto* p : res1) { p->print(std::cout); std::cout << "\n"; }

                std::cout << "На втором складе найдено: " << res2.size() << "\n";
                for (auto* p : res2) { p->print(std::cout); std::cout << "\n"; }
                break;
            }
            case 9: {
                int p_val = input_int("Введите искомую цену: ");
                auto res1 = main_wh->find_by_price(Price(p_val));
                auto res2 = second_wh->find_by_price(Price(p_val));

                std::cout << "\n--- Результаты поиска ---\n";
                std::cout << "На основном складе найдено: " << res1.size() << "\n";
                for (auto* p : res1) { p->print(std::cout); std::cout << "\n"; }

                std::cout << "На втором складе найдено: " << res2.size() << "\n";
                for (auto* p : res2) { p->print(std::cout); std::cout << "\n"; }
                break;
            }
            case 10: {
                Warehouse* wh = select_warehouse(main_wh.get(), second_wh.get());
                if (!wh) break;

                std::string id = input_string("Введите ID товара: ");
                Product* p = wh->find_by_id(id);
                if (p) {
                    int new_p = input_int("Введите новую цену: ");
                    p->set_price(Price(new_p));
                    std::cout << "Цена обновлена.\n";
                }
                else std::cout << "Товар не найден.\n";
                break;
            }
            case 11: {
                Warehouse* wh = select_warehouse(main_wh.get(), second_wh.get());
                if (!wh) break;

                std::string id = input_string("Введите ID товара: ");
                Product* p = wh->find_by_id(id);
                if (p) {
                    int delta = input_int("Изменение количества (например, -5): ");
                    p->change_quantity(delta);
                    std::cout << "Количество обновлено.\n";
                }
                else std::cout << "Товар не найден.\n";
                break;
            }
            case 12: {
                std::string id = input_string("Введите ID товара для перемещения на второй склад: ");
                Product* p = main_wh->find_by_id(id);
                if (p) {
                    p->transfer_to(*second_wh); 
                    main_wh->remove_by_id(id);  
                    std::cout << "Товар перемещен.\n";
                }
                else std::cout << "Товар не найден на основном складе.\n";
                break;
            }
            case 0: std::cout << "Выход.\n"; break;
            default: std::cout << "Неверный пункт.\n"; break;
            }
        }
        catch (const std::exception& e) {
            std::cout << "\n[ОШИБКА]: " << e.what() << "\n";
        }
    } while (choice != 0);

    return 0;
}