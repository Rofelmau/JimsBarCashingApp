#include "Cocktail.h"

#include <QUuid>

Cocktail::Cocktail(const QString &uuid, const QString &name)
    : uuid(uuid.isEmpty() ? QUuid::createUuid().toString() : uuid)
    , name(name)
{
}

Cocktail::Cocktail(const Cocktail &other)
{
    uuid = other.uuid;
    name = other.name;
    ingredients = other.ingredients;
}

Cocktail &Cocktail::operator=(const Cocktail &other)
{
    if (this != &other) {
        uuid = other.uuid;
        name = other.name;
        ingredients = other.ingredients;
    }
    return *this;
}

QString Cocktail::getName() const
{
    return name;
}

QStringList Cocktail::getIngredients() const
{
    return ingredients;
}

void Cocktail::setIngredients(const QStringList &ingredients)
{
    this->ingredients = ingredients;
}

QString Cocktail::getUuid() const
{
    return uuid;
}

bool Cocktail::operator==(const Cocktail &other) const
{
    return uuid == other.getUuid();
}

uint qHash(const Cocktail &cocktail, uint seed)
{
    return qHash(cocktail.getUuid(), seed) ^ qHash(cocktail.getName(), seed+1);
}

