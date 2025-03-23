#include "Discount.h"

Discount::Discount()
    : m_id(0)
    , m_name("")
    , m_value(0.0)
    , m_cocktailLimit(0)
{
}

Discount::Discount(int id, const QString &name, DiscountType type, double value, int cocktailLimit)
    : m_id(id)
    , m_name(name)
    , m_type(type)
    , m_value(value)
    , m_cocktailLimit(cocktailLimit)
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
