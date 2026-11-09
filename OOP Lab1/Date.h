#pragma once
#include <string>

class Date {
private:
    int day;
    int month;
    int year;

    bool is_valid_date(int d, int m, int y) const;
    static bool is_leap_year(int y);

public:
    Date();
    Date(int d, int m, int y);
    explicit Date(const std::string& date_str);

    int get_day() const;
    int get_month() const;
    int get_year() const;

    Date add_days(int days) const;
    std::string to_string() const;

    bool operator<(const Date& other) const;
    bool operator>(const Date& other) const;
    bool operator==(const Date& other) const;
    bool operator<=(const Date& other) const;
    bool operator>=(const Date& other) const;
};