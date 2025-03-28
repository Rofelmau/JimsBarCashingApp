#pragma once

#include "Cocktail.h"
#include "Discount.h"

#include <QDateTime>
#include <QPair>
#include <QSet>
#include <QSharedPointer>
#include <QVector>
#include <QMap>
#include <QHash>

enum class PaymentMethod {
    Undefined = -1,
    Cash = 0,
    Card
};

class SaleDetail {
public:
    SaleDetail(const QString &cocktailUuid, int quantity);

    QString getCocktailUuid() const;
    int getQuantity() const;
    void setQuantity(int quantity);

private:
    QString cocktailUuid;
    int quantity;
};

class Sale {
public:
    Sale(int id = -1
        , const QDateTime &timestamp = QDateTime::currentDateTime()
        , PaymentMethod paymentMethod = PaymentMethod::Undefined
        , double pricePerCocktail = 0.0
        , double totalPrice = 0.0);

    int getId() const;
    QDateTime getTimestamp() const;
    double getPricePerCocktail() const;
    double getTotalPrice() const;
    double getTotalCupPawn() const;
    QVector<SaleDetail> getDetails() const;
    PaymentMethod getPaymentMethod() const;

    double calculateDiscount(double totalCocktailPrice);
    void updateTotalPrice();

    void setPricePerCocktail(double price);
    void setTotalPrice(double price);
    void setCupPawn(double pawn);
    void addDetail(const SaleDetail &detail);
    void setPaymentMethod(PaymentMethod method);

    void incrementQuantity(const QString &cocktailUuid);
    void decrementQuantity(const QString &cocktailUuid);
    void incrementReturnedCups();
    int getReturnedCups() const;

    void incerementDiscountQuantity(QSharedPointer<Discount> discount);
    void decrementDiscountQuantity(QSharedPointer<Discount> discount);
    QSet<QSharedPointer<Discount>> getAppliedDiscounts() const;
    int getDiscountQuantity(const QString &discountUuid) const;

    int getTotalCocktailCount() const;

private:
    int id;
    QDateTime timestamp;
    double pricePerCocktail;
    double totalPrice;
    double cupPawn;
    QVector<SaleDetail> details;
    PaymentMethod m_paymentMethod;
    int returnedCups = 0;

    QHash<QString, int> discountQuantities;
    QHash<QString, QSharedPointer<Discount>> discountLookup;
};
