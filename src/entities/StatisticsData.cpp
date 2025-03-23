#include "StatisticsData.h"

QString StatisticsData::getCocktailName() const
{
    return cocktailName;
}

int StatisticsData::getQuantitySold() const
{
    return quantitySold;
}

double StatisticsData::getPricePerCocktail() const
{
    return pricePerCocktail;
}

double StatisticsData::getTotalPrice() const
{
    return totalPrice;
}

void StatisticsData::setCocktailName(const QString &name)
{
    cocktailName = name;
}

void StatisticsData::setQuantitySold(int quantity)
{
    quantitySold = quantity;
}

void StatisticsData::setPricePerCocktail(double price)
{
    pricePerCocktail = price;
}

void StatisticsData::setTotalPrice(double total)
{
    totalPrice = total;
}
