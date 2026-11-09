#include "Customer.h"
#include <stdexcept>

Customer::Customer(const std::string& id, const std::string& first_name, const std::string& last_name, const std::string& region)
    : id(id), first_name(first_name), last_name(last_name), region(region), wallet_balance(0.0), bonus_points(0) {
}

std::string Customer::get_id() const { return id; }
std::string Customer::get_full_name() const { return first_name + " " + last_name; }
std::string Customer::get_region() const { return region; }
double Customer::get_wallet_balance() const { return wallet_balance; }
int Customer::get_bonus_points() const { return bonus_points; }

void Customer::add_funds(double amount) {
    if (amount < 0) throw std::invalid_argument("Сумма пополнения не может быть отрицательной");
    wallet_balance += amount;
}

void Customer::deduct_funds(double amount) {
    if (amount < 0) throw std::invalid_argument("Сумма списания не может быть отрицательной");
    if (wallet_balance < amount) throw std::runtime_error("Недостаточно средств на кошельке");
    wallet_balance -= amount;
}

void Customer::add_bonus_points(int points) {
    if (points < 0) throw std::invalid_argument("Количество баллов не может быть отрицательным");
    bonus_points += points;
}

void Customer::spend_bonus_points(int points) {
    if (points < 0) throw std::invalid_argument("Количество баллов не может быть отрицательным");
    if (bonus_points < points) throw std::runtime_error("Недостаточно бонусных баллов");
    bonus_points -= points;
}