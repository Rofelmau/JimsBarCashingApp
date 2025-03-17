#include "GeneralSettings.h"

GeneralSettings::GeneralSettings()
    : pricePerCocktail(0.0)
    , cupPawn(0.0)
{
    for (auto &cocktail : selectedCocktails)
    {
        cocktail = nullptr;
    }
}

double GeneralSettings::getPricePerCocktail() const
{
    return pricePerCocktail;
}

double GeneralSettings::getCupPawn() const
{
    return cupPawn;
}

QSharedPointer<Cocktail> GeneralSettings::getSelectedCocktail(int index) const
{
    if (index >= 1 && index <= 6)
    {
        return selectedCocktails[index - 1];
    }
    return nullptr;
}

QVector<QSharedPointer<Cocktail>> GeneralSettings::getSelectedCocktails() const
{
    QVector<QSharedPointer<Cocktail>> cocktails;
    for (auto &cocktail : selectedCocktails)
    {
        cocktails.push_back(cocktail);
    }
    return cocktails;
}

void GeneralSettings::setPricePerCocktail(const double price)
{
    pricePerCocktail = price;
}

void GeneralSettings::setCupPawn(const double pawn)
{
    cupPawn = pawn;
}

void GeneralSettings::setSelectedCocktail(const int index, QSharedPointer<Cocktail> cocktail)
{
    if (index >= 1 && index <= 6)
    {
        selectedCocktails[index - 1] = cocktail;
    }
}
