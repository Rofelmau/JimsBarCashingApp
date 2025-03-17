#pragma once

#include <QString>
#include <QStringList>

class Cocktail
{
public:
    Cocktail(int id = -1, const QString &name = "");

    Cocktail(const Cocktail &other);
    Cocktail &operator=(const Cocktail &other);
    bool operator==(const Cocktail &other) const;

    int getId() const;
    QString getName() const;
    QStringList getIngredients() const;
    void setIngredients(const QStringList &ingredients);

private:
    int id;
    QString name;
    QStringList ingredients;
};

uint qHash(const Cocktail &cocktail, uint seed = 0);
