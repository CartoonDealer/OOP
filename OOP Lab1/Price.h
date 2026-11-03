#pragma once

class Price {
private:
    int value;

public:
    explicit Price(int val);
    Price();

    int get_value() const;
    void set_value(int new_val);
};