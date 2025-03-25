#include "Discount.h"

Discount::Discount()
    : m_id(0)
    , m_name("")
    , m_value(0.0)
    , m_cocktailLimit(0)
    , m_active(false)
{
}

Discount::Discount(int id, const QString &name, DiscountType type, double value, int cocktailLimit, bool active)
    : m_id(id)
    , m_name(name)
    , m_type(type)
    , m_value(value)
    , m_cocktailLimit(cocktailLimit)
    , m_active(active)
{
}

int Discount::getId() const
{
    return m_id;
}

void Discount::setId(int id)
{
    m_id = id;
}

QString Discount::getName() const
{
    return m_name;
}

void Discount::setName(const QString &name)
{
    m_name = name;
}

DiscountType Discount::getType() const
{
    return m_type;
}

void Discount::setType(DiscountType type)
{
    m_type = type;
}

double Discount::getValue() const
{
    return m_value;
}

void Discount::setValue(double value)
{
    m_value = value;
}

int Discount::getCocktailLimit() const
{
    return m_cocktailLimit;
}

void Discount::setCocktailLimit(int cocktailLimit)
{
    m_cocktailLimit = cocktailLimit;
}

bool Discount::isActive() const
{
    return m_active;
}

void Discount::setActive(bool active)
{
    m_active = active;
}

QString Discount::getInfoText() const
{
    QString infoText{"(%1)"};
    switch (m_type) {
        case DiscountType::ClassicDiscount: {
            QString extraInfo{"%0€ Rabatt auf 1 Cocktail"};
            extraInfo = extraInfo.arg(QString::number(m_value));
            infoText = infoText.arg(extraInfo);
            break;
        }
        case DiscountType::GroupDiscount: {
            QString extraInfo{"%0 Cocktails für %1€"};
            extraInfo = extraInfo.arg(QString::number(m_cocktailLimit)).arg(QString::number(m_value));
            infoText = infoText.arg(extraInfo);
            break;
        }
        case DiscountType::ForFree: {
            QString extraInfo{"1 Cocktial für 0€"};
            infoText = infoText.arg(extraInfo);
            break;
        }
        case DiscountType::PercentageDiscount: {
            QString extraInfo{"%0% Rabatt auf 1 Cocktail"};
            extraInfo = extraInfo.arg(QString::number(m_value));
            infoText = infoText.arg(extraInfo);
            break;
        }
    }

    return infoText;
}