#include "HouseholdChemical.h"

HouseholdChemical::HouseholdChemical(const std::string& id, const std::string& name, const std::string& manufacturer,
    const Price& price, const Quantity& quantity, const Weight& weight,
    const Date& production_date, int shelf_life_days, bool is_toxic)
    : PerishableProduct(id, name, manufacturer, price, quantity, weight, production_date, shelf_life_days),
    is_toxic(is_toxic) {
}

bool HouseholdChemical::get_is_toxic() const { return is_toxic; }

std::unique_ptr<Product> HouseholdChemical::clone() const {
    return std::make_unique<HouseholdChemical>(*this);
}

void HouseholdChemical::print(std::ostream& os) const {
    os << "[Химия] ID: " << id << " | " << name
        << " (Произв: " << manufacturer << ")"
        << " | Цена: " << price.get_value() << " руб. | Кол-во: " << quantity.get_value() << " шт."
        << " | Годен до: " << get_expiration_date().to_string()
        << (is_toxic ? " | [ОПАСНО!]" : " | [Безопасно]");
}