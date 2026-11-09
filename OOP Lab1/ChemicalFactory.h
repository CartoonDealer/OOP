#pragma once
#include "ProductFactory.h"
#include "HouseholdChemical.h"
#include <memory>
#include <string>

class ChemicalFactory : public ProductFactory {
public:
    std::shared_ptr<Product> create_product(const std::string& id, const std::string& name,
        const std::string& manufacturer, 
        int price, int quantity, double weight,
        const Date& production_date, int shelf_life_days,
        bool is_toxic) const;
};