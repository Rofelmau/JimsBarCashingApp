#include "Discount.h"

#include <QUuid>

Discount::Discount(const QString &uuid,
                    const QString &name,
                    DiscountType type,
                    double value,
                    int cocktailLimit,
                    bool active)
    : m_uuid(uuid.isEmpty() ? QUuid::createUuid().toString() : uuid)
    , m_name(name)
    , m_type(type)
    , m_value(value)
    , m_cocktailLimit(cocktailLimit)
    , m_active(active)
{
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

QString Discount::getUuid() const
{
    return m_uuid;
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
            QString extraInfo{"1 Cocktail für 0€"};
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