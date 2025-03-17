#pragma once

#include <QDateTime>
#include <QVector>
#include <QSharedPointer>
#include "Cocktail.h"

enum class PaymentMethod {
    Undefined = -1,
    Cash = 0,
    Card
};

class SaleDetail {
public:
    SaleDetail(int cocktailId, int quantity);

    int getCocktailId() const;
    int getQuantity() const;
    void setQuantity(int quantity);

private:
    int cocktailId;
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

    void updateTotalPrice();

    void setPricePerCocktail(double price);
    void setTotalPrice(double price);
    void setCupPawn(double pawn);
    void addDetail(const SaleDetail &detail);
    void setPaymentMethod(PaymentMethod method);

    void incrementQuantity(int cocktailId);
    void decrementQuantity(int cocktailId);
    void incrementReturnedCups();
    int getReturnedCups() const;

private:
    int id;
    QDateTime timestamp;
    double pricePerCocktail;
    double totalPrice;
    double cupPawn;
    QVector<SaleDetail> details;
    PaymentMethod m_paymentMethod;
    int returnedCups = 0;
};
