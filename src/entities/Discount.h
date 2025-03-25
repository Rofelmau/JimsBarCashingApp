#pragma once

#include "DiscountType.h"

#include <QString>

class Discount {
public:
    explicit Discount();
    Discount(int id, const QString &name, DiscountType type, double value = -1.0, int cocktailLimit = -1, bool active = false);

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

    bool isActive() const;
    void setActive(bool active);

    QString getInfoText() const;
private:
    int m_id;
    QString m_name;
    DiscountType m_type;
    double m_value;
    int m_cocktailLimit;
    bool m_active;
};
