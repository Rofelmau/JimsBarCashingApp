#pragma once

#include <QString>
#include <QStringList>

class Cocktail
{
public:
    explicit Cocktail(const QString &uuid = "", const QString &name = "");

    Cocktail(const Cocktail &other);
    Cocktail &operator=(const Cocktail &other);
    bool operator==(const Cocktail &other) const;

    QString getName() const;
    QStringList getIngredients() const;
    void setIngredients(const QStringList &ingredients);

    QString getUuid() const;

private:
    QString uuid;
    QString name;
    QStringList ingredients;
};

uint qHash(const Cocktail &cocktail, uint seed = 0);
