#pragma once

class Weight {
private:
    double value;

public:
    explicit Weight(double val);
    Weight();

    double get_value() const;
    void set_value(double new_val);
};