#include "Cocktail.h"

#include <QDebug>

Cocktail::Cocktail(int id, const QString &name)
    : id(id), name(name)
{
}

Cocktail::Cocktail(const Cocktail &other)
{
    qWarning() << "Custom copy constructor called! Copying ID:" << other.id;
    id = other.id;
    name = other.name;  // QString macht eine eigene Speicherverwaltung
    ingredients = other.ingredients;
}

Cocktail &Cocktail::operator=(const Cocktail &other)
{
    qWarning() << "Custom assignment operator called!";
    if (this != &other) {
        id = other.id;
        name = other.name;
        ingredients = other.ingredients;
    }
    return *this;
}

int Cocktail::getId() const
{
    return id;
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

bool Cocktail::operator==(const Cocktail &other) const
{
    qWarning() << "operator== called";
    return id == other.getId() && name.compare(other.getName()) == 0;
}

uint qHash(const Cocktail &cocktail, uint seed)
{
    qWarning() << "qHash called";
    return qHash(cocktail.getId(), seed) ^ qHash(cocktail.getName(), seed+1);
}

