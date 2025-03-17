#pragma once

#include <QString>
#include <QSharedPointer>
#include "Cocktail.h"

class GeneralSettings
{
public:
    explicit GeneralSettings();

    double getPricePerCocktail() const;
    double getCupPawn() const;
    QSharedPointer<Cocktail> getSelectedCocktail(int index) const;
    QVector<QSharedPointer<Cocktail>> getSelectedCocktails() const;

    void setPricePerCocktail(double price);
    void setCupPawn(double pawn);
    void setSelectedCocktail(int index, QSharedPointer<Cocktail> cocktail);

private:
    double pricePerCocktail;
    double cupPawn;
    QSharedPointer<Cocktail> selectedCocktails[6];
};
