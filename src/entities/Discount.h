#pragma once

#include "DiscountType.h"

#include <QString>

class Discount {
public:
    explicit Discount();
    Discount(int id, const QString &name, DiscountType type, double value, int cocktailLimit, bool reusable, bool combinable);

    int getId() const;
    void setId(int id);

    QString getName() const;
    void setName(const QString &name);

    DiscountType getType() const;
    void setType(DiscountType type);

    double getValue() const;
    void setValue(double value);

    int getCocktailLimit() const;
    void setCocktailLimit(int cocktailLimit);

    bool isReusable() const;
    void setReusable(bool reusable);

    bool isCombinable() const;
    void setCombinable(bool combinable);

private:
    int m_id;
    QString m_name;
    DiscountType m_type;
    double m_value;
    int m_cocktailLimit;
    bool m_reusable;
    bool m_combinable;
};
