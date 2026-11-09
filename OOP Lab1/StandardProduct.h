#pragma once
#include "Product.h"

class StandardProduct : public Product {
public:
    StandardProduct(const std::string& id, const std::string& name, const std::string& manufacturer,
        const Price& price, const Quantity& quantity, const Weight& weight);

    std::unique_ptr<Product> clone() const override;
    void print(std::ostream& os) const override;
};