#pragma once

#include "../entities/Sale.h"

#include "../repositories/CocktailRepository.h"
#include "../repositories/SalesRepository.h"
#include "../repositories/SettingsRepository.h"

#include <QObject>
#include <QSharedPointer>
#include <QStringList>
#include <QVariantList>

class CheckoutScreenController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(double pricePerCocktail READ pricePerCocktail NOTIFY pricePerCocktailChanged)
    Q_PROPERTY(double cupPawn READ cupPawn NOTIFY pricePerCocktailChanged)
    Q_PROPERTY(QVariantList selectedCocktails READ selectedCocktails NOTIFY selectedCocktailsChanged)

    Q_PROPERTY(QVariantList currentSaleDetails READ currentSaleDetails NOTIFY currentSaleUpdated)
    Q_PROPERTY(double totalPrice READ totalPrice NOTIFY currentSaleUpdated)
    Q_PROPERTY(double totalCupPawn READ totalCupPawn NOTIFY currentSaleUpdated)
    Q_PROPERTY(int returnedCups READ returnedCups NOTIFY currentSaleUpdated)

public:
    explicit CheckoutScreenController(QSharedPointer<SettingsRepository> settingsRepo
                                        , QSharedPointer<CocktailRepository> cocktailRepo
                                        , QSharedPointer<SalesRepository> salesRepo
                                        , QObject *parent = nullptr);

    double pricePerCocktail() const;
    double cupPawn() const;
    QVariantList selectedCocktails() const;
    QVariantList currentSaleDetails() const;
    double totalPrice() const;
    double totalCupPawn() const;
    int returnedCups() const;

    Q_INVOKABLE void cocktailSelected(int cocktailId);
    Q_INVOKABLE void decreaseQuantity(int cocktailId);
    Q_INVOKABLE void confirmPayment(int paymentMethod);
    Q_INVOKABLE void returnCup();

signals:
    void pricePerCocktailChanged();
    void selectedCocktailsChanged();
    void currentSaleUpdated();

private:
    QSharedPointer<SettingsRepository> m_settingsRepository;
    QSharedPointer<CocktailRepository> m_cocktailRepository;
    QSharedPointer<SalesRepository> m_salesRepository;;

    Sale m_currentSale;
};
