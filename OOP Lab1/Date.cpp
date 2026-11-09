#include "Date.h"
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <ctime>

Date::Date() : day(1), month(1), year(2026) {}

Date::Date(int d, int m, int y) : day(d), month(m), year(y) {
    if (!is_valid_date(d, m, y)) {
        throw std::invalid_argument("Некорректная дата");
    }
}

Date::Date(const std::string& date_str) {
    std::istringstream ss(date_str);
    char dot1, dot2;
    if (!(ss >> day >> dot1 >> month >> dot2 >> year) || dot1 != '.' || dot2 != '.') {
        throw std::invalid_argument("Неверный формат даты. Ожидается ДД.ММ.ГГГГ");
    }
    if (!is_valid_date(day, month, year)) {
        throw std::invalid_argument("Некорректная дата");
    }
}

bool Date::is_leap_year(int y) {
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

bool Date::is_valid_date(int d, int m, int y) const {
    if (y < 1900 || m < 1 || m > 12 || d < 1) return false;
    int days_in_month[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (m == 2 && is_leap_year(y)) return d <= 29;
    return d <= days_in_month[m];
}

int Date::get_day() const { return day; }
int Date::get_month() const { return month; }
int Date::get_year() const { return year; }

Date Date::add_days(int days) const {
    std::tm tm = {};
    tm.tm_mday = day + days;
    tm.tm_mon = month - 1;
    tm.tm_year = year - 1900;

    std::mktime(&tm);
    return Date(tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
}

std::string Date::to_string() const {
    std::ostringstream os;
    os << std::setfill('0') << std::setw(2) << day << "."
        << std::setfill('0') << std::setw(2) << month << "."
        << year;
    return os.str();
}

bool Date::operator<(const Date& other) const {
    if (year != other.year) return year < other.year;
    if (month != other.month) return month < other.month;
    return day < other.day;
}

bool Date::operator>(const Date& other) const { return other < *this; }
bool Date::operator==(const Date& other) const { return year == other.year && month == other.month && day == other.day; }
bool Date::operator<=(const Date& other) const { return *this < other || *this == other; }
bool Date::operator>=(const Date& other) const { return *this > other || *this == other; }