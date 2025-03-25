#include "CocktailsConfigurationScreenController.h"

#include <Cocktail.h>

#include "../Logger.h"

#include <algorithm>

CocktailsConfigurationScreenController::CocktailsConfigurationScreenController(QSharedPointer<CocktailRepository> cocktailRepository, QObject *parent)
    : QObject(parent)
    , m_cocktailRepository(cocktailRepository)
{
}

QVariantList CocktailsConfigurationScreenController::cocktails() const
{
    QVariantList cocktailList;
    QSet<QSharedPointer<Cocktail>> cocktails = m_cocktailRepository->getAllCocktails();
    QList<QSharedPointer<Cocktail>> sortedCocktails = cocktails.values();
    
    std::sort(sortedCocktails.begin(), sortedCocktails.end(), [](const QSharedPointer<Cocktail> &a, const QSharedPointer<Cocktail> &b) {
        return a->getName().toLower() < b->getName().toLower();
    });

    for (const QSharedPointer<Cocktail> &cocktail : sortedCocktails) {
        QVariantMap cocktailMap;
        cocktailMap["id"] = cocktail->getId();
        cocktailMap["name"] = cocktail->getName();
        cocktailMap["ingredients"] = cocktail->getIngredients();
        cocktailList.append(cocktailMap);
    }
    return cocktailList;
}

void CocktailsConfigurationScreenController::editCocktail(const int id, const QString &name, const QStringList &ingredients) {
    if (name.isEmpty()) {
        Logger::LogWarn("Cocktail name cannot be empty!");
        return;
    }

    m_cocktailRepository->editCocktail(id, name, ingredients);
    emit cocktailsChanged();
}

void CocktailsConfigurationScreenController::deleteCocktail(const int id)
{
    m_cocktailRepository->deleteCocktail(id);
    emit cocktailsChanged();
}

void CocktailsConfigurationScreenController::addNewCocktail(const QString &name, const QStringList &ingredients)
{
    if (name.isEmpty()) {
        Logger::LogWarn("Cocktail name cannot be empty!");
        return;
    }
    m_cocktailRepository->addCocktail(name, ingredients);
    emit cocktailsChanged();
}
