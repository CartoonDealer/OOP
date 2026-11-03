#pragma once

class Quantity {
private:
    int value;

public:
    explicit Quantity(int val);
    Quantity();

    int get_value() const;
    void set_value(int new_val);
    void change(int delta);
};