#pragma once
#include "PerishableProduct.h"

class HouseholdChemical : public PerishableProduct {
private:
    bool is_toxic;

public:
    HouseholdChemical(const std::string& id, const std::string& name, const std::string& manufacturer,
        const Price& price, const Quantity& quantity, const Weight& weight,
        const Date& production_date, int shelf_life_days, bool is_toxic);

    bool get_is_toxic() const;

    std::unique_ptr<Product> clone() const override;
    void print(std::ostream& os) const override;
};