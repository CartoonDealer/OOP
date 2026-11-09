#pragma once
#include <string>

class Customer {
private:
    std::string id;
    std::string first_name;
    std::string last_name;
    std::string region; 
    double wallet_balance;
    int bonus_points;

public:
    Customer(const std::string& id, const std::string& first_name, const std::string& last_name, const std::string& region);

    std::string get_id() const;
    std::string get_full_name() const;
    std::string get_region() const; 
    double get_wallet_balance() const;
    int get_bonus_points() const;

    void add_funds(double amount);
    void deduct_funds(double amount);
    void add_bonus_points(int points);
    void spend_bonus_points(int points);
};