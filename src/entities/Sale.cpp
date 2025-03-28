#include "Sale.h"

#include "../Logger.h"

#include <cmath>

SaleDetail::SaleDetail(const QString &cocktailUuid, int quantity)
    : cocktailUuid(cocktailUuid)
    , quantity(quantity)
{
}

QString SaleDetail::getCocktailUuid() const
{
    return cocktailUuid;
}

int SaleDetail::getQuantity() const
{
    return quantity;
}

void SaleDetail::setQuantity(int quantity)
{
    this->quantity = quantity;
}

Sale::Sale(int id, const QDateTime &timestamp, PaymentMethod paymentMethod, double pricePerCocktail, double totalPrice)
    : id(id)
    , timestamp(timestamp)
    , m_paymentMethod(paymentMethod)
    , pricePerCocktail(pricePerCocktail)
    , totalPrice(totalPrice)
{
}

int Sale::getId() const
{
    return id;
}

QDateTime Sale::getTimestamp() const
{
    return timestamp;
}

double Sale::getPricePerCocktail() const
{
    return pricePerCocktail;
}

double Sale::getTotalPrice() const
{
    return totalPrice;
}

QVector<SaleDetail> Sale::getDetails() const
{
    return details;
}

double Sale::calculateDiscount(const double totalCocktailPrice)
{
    double discountAmount = 0.0;
    const int totalCocktails = getTotalCocktailCount();

    for (auto it = discountQuantities.begin(); it != discountQuantities.end(); ++it) {
        const QString &discountUuid = it.key();
        const int quantity = it.value();

        if (!discountLookup.contains(discountUuid)) {
            continue;
        }

        QSharedPointer<Discount> discount = discountLookup.value(discountUuid);

        switch (discount->getType()) {
            case DiscountType::ClassicDiscount: {
                const int actualQuantity = std::min(totalCocktails, quantity);
                discountAmount += discount->getValue() * actualQuantity;
                break;
            }
            case DiscountType::GroupDiscount: {
                const int groupSize = discount->getCocktailLimit();
                const int groupCount = std::floor(totalCocktails / groupSize);
                if (groupCount <= 0) {
                    break;
                }
                const int actualQuantity = std::min(groupCount, quantity);
                const double priceWithoutDiscount = pricePerCocktail * actualQuantity * groupSize;
                const double discountedPrice = discount->getValue() * actualQuantity;
                discountAmount += priceWithoutDiscount - discountedPrice;
                break;
            }
            case DiscountType::ForFree: {
                const int actualQuantity = std::min(totalCocktails, quantity);
                discountAmount += pricePerCocktail * actualQuantity;
                break;
            }
            case DiscountType::PercentageDiscount: {
                const int actualQuantity = std::min(totalCocktails, quantity);
                discountAmount += (pricePerCocktail * discount->getValue() / 100) * actualQuantity;
                break;
            }
        }
    }

    return discountAmount;
}

void Sale::updateTotalPrice()
{
    double totalCocktailPrice = 0.0;
    for (const auto &detail : details) {
        totalCocktailPrice += detail.getQuantity() * pricePerCocktail;
    }

    const double cupPawn = getTotalCupPawn();
    const double cupReturnRefund = returnedCups * this->cupPawn;

    const double discountAmount = calculateDiscount(totalCocktailPrice);

    totalPrice = totalCocktailPrice + cupPawn - cupReturnRefund - discountAmount;
}

void Sale::setPricePerCocktail(double price)
{
    pricePerCocktail = price;
    updateTotalPrice();
}

void Sale::setTotalPrice(double price)
{
    totalPrice = price;
}

void Sale::setCupPawn(double pawn)
{
    cupPawn = pawn;
    updateTotalPrice();
}

void Sale::setPaymentMethod(const PaymentMethod method)
{
    m_paymentMethod = method;
}

PaymentMethod Sale::getPaymentMethod() const
{
    return m_paymentMethod;
}

void Sale::addDetail(const SaleDetail &detail)
{
    details.append(detail);
    updateTotalPrice();
}

void Sale::incrementQuantity(const QString &cocktailUuid)
{
    auto it = std::find_if(details.begin(), details.end(), [cocktailUuid](const SaleDetail &detail) {
        return detail.getCocktailUuid() == cocktailUuid;
    });

    if (it != details.end()) {
        it->setQuantity(it->getQuantity() + 1);
    } else {
        details.append(SaleDetail(cocktailUuid, 1));
    }

    updateTotalPrice();
}

void Sale::decrementQuantity(const QString &cocktailUuid)
{
    auto it = std::find_if(details.begin(), details.end(), [cocktailUuid](const SaleDetail &detail) {
        return detail.getCocktailUuid() == cocktailUuid;
    });

    if (it != details.end()) {
        if (it->getQuantity() > 1) {
            it->setQuantity(it->getQuantity() - 1);
        } else {
            details.erase(it);
        }
    }

    updateTotalPrice();
}

double Sale::getTotalCupPawn() const
{
    int cupsRequired = 0;
    for (const auto &detail : details) {
        cupsRequired += detail.getQuantity();
    }
    return cupsRequired * cupPawn;
}

void Sale::incrementReturnedCups()
{
    returnedCups++;
    updateTotalPrice();
}

int Sale::getReturnedCups() const
{
    return returnedCups;
}

void Sale::incerementDiscountQuantity(QSharedPointer<Discount> discount)
{
    if (!discount) {
        return;
    }

    const QString& discountUuid = discount->getUuid();
    if (!discountLookup.contains(discountUuid)) {
        discountLookup.insert(discountUuid, discount);
        discountQuantities.insert(discountUuid, 1);
    } else {
        discountQuantities[discountUuid]++;
    }

    updateTotalPrice();
}

void Sale::decrementDiscountQuantity(QSharedPointer<Discount> discount)
{
    if (!discount) {
        return;
    }

    const QString& discountUuid = discount->getUuid();
    if (discountQuantities.contains(discountUuid)) {
        if (discountQuantities[discountUuid] > 1) {
            discountQuantities[discountUuid]--;
        } else {
            discountQuantities.remove(discountUuid);
            discountLookup.remove(discountUuid);
        }
    }

    updateTotalPrice();
}

QSet<QSharedPointer<Discount>> Sale::getAppliedDiscounts() const
{
    QSet<QSharedPointer<Discount>> appliedDiscounts;
    for (auto it = discountLookup.begin(); it != discountLookup.end(); ++it) {
        appliedDiscounts.insert(it.value());
    }
    return appliedDiscounts;
}

int Sale::getDiscountQuantity(const QString &discountUuid) const
{
    return discountQuantities.value(discountUuid, 0);
}

int Sale::getTotalCocktailCount() const
{
    int totalCocktails = 0;
    for (const auto &detail : details) {
        totalCocktails += detail.getQuantity();
    }
    return totalCocktails;
}
