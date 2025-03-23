#pragma once

#include <QString>

class StatisticsData {
public:
    // Getters
    QString getCocktailName() const;
    int getQuantitySold() const;
    double getPricePerCocktail() const;
    double getTotalPrice() const;

    // Setters
    void setCocktailName(const QString &name);
    void setQuantitySold(int quantity);
    void setPricePerCocktail(double price);
    void setTotalPrice(double total);

private:
    QString cocktailName;
    int quantitySold;
    double pricePerCocktail;
    double totalPrice;
};
