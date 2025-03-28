#pragma once

#include "DiscountType.h"

#include <QString>

class Discount {
public:
    explicit Discount(const QString &uuid = "",
        const QString &name = "",
        DiscountType type = DiscountType::ClassicDiscount,
        double value = -1.0,
        int cocktailLimit = -1,
        bool active = false
    );

    QString getName() const;
    DiscountType getType() const;
    double getValue() const;
    int getCocktailLimit() const;
    bool isActive() const;
    QString getUuid() const;

    void setName(const QString &name);
    void setType(DiscountType type);
    void setValue(double value);
    void setCocktailLimit(int cocktailLimit);
    void setActive(bool active);

    QString getInfoText() const;

private:
    QString m_uuid;
    QString m_name;
    DiscountType m_type;
    double m_value;
    int m_cocktailLimit;
    bool m_active;
};
