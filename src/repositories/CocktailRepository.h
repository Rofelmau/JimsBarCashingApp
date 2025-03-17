#pragma once

#include "../DatabaseManager.h"

#include <QObject>
#include <QSet>
#include <QSharedPointer>
#include <QString>
#include <QStringList>
#include <QVariant>

class Cocktail;

class CocktailRepository : public QObject
{
    Q_OBJECT
public:
    explicit CocktailRepository(QSharedPointer<DatabaseManager> dbManager, QObject *parent = nullptr);

    QSet<QSharedPointer<Cocktail>> getAllCocktails() const;
    QSharedPointer<Cocktail> getCocktailById(int id) const;

    void addCocktail(const QString &name, const QStringList &ingredients) const;
    void deleteCocktail(int id) const;
    void editCocktail(int id, const QString &name, const QStringList &ingredients) const;

private:
    void loadCocktailsIntoCache() const;

    mutable QSet<QSharedPointer<Cocktail>> m_cachedCocktails;
    QSharedPointer<DatabaseManager> dbManager;
};
