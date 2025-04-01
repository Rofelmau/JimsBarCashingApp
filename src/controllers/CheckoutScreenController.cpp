#include "CheckoutScreenController.h"

#include "../entities/Sale.h"
#include "../repositories/CashBalanceRepository.h"

CheckoutScreenController::CheckoutScreenController(QSharedPointer<SettingsRepository> settingsRepo
                                                    , QSharedPointer<CocktailRepository> cocktailRepo
                                                    , QSharedPointer<SalesRepository> salesRepo
                                                    , QSharedPointer<DiscountsRepository> discountsRepo
                                                    , QSharedPointer<CashBalanceRepository> cashBalanceRepo
                                                    , QObject *parent)
    : QObject(parent)
    , m_settingsRepository(settingsRepo)
    , m_cocktailRepository(cocktailRepo)
    , m_salesRepository(salesRepo)
    , m_discountsRepository(discountsRepo)
    , m_cashBalanceRepository(cashBalanceRepo)
{
    m_currentSale.setPricePerCocktail(m_settingsRepository->getGeneralSettings()->getPricePerCocktail());
    m_currentSale.setCupPawn(m_settingsRepository->getGeneralSettings()->getCupPawn());
}

double CheckoutScreenController::pricePerCocktail() const
{
    return m_settingsRepository->getGeneralSettings()->getPricePerCocktail();
}

double CheckoutScreenController::cupPawn() const
{
    return m_settingsRepository->getGeneralSettings()->getCupPawn();
}

QVariantList CheckoutScreenController::selectedCocktails() const
{
    QVariantList cocktailsList;
    auto cocktails = m_settingsRepository->getGeneralSettings()->getSelectedCocktails();

    for (const auto &cocktail : cocktails) {
        if (cocktail) {
            QVariantMap cocktailData;
            cocktailData["uuid"] = cocktail->getUuid();
            cocktailData["name"] = cocktail->getName();
            cocktailsList.append(cocktailData);
        }
    }

    return cocktailsList;
}

void CheckoutScreenController::cocktailSelected(const QString &cocktailUuid)
{
    auto cocktail = m_cocktailRepository->getCocktailByUuid(cocktailUuid);
    if (cocktail) {
        m_currentSale.incrementQuantity(cocktail->getUuid());
        emit currentSaleUpdated();
    }
}

void CheckoutScreenController::decreaseQuantity(const QString &cocktailUuid)
{
    auto cocktail = m_cocktailRepository->getCocktailByUuid(cocktailUuid);
    if (cocktail) {
        m_currentSale.decrementQuantity(cocktail->getUuid());
        emit currentSaleUpdated();
    }
}

QVariantList CheckoutScreenController::currentSaleDetails() const
{
    QVariantList detailsList;
    for (const SaleDetail &detail : m_currentSale.getDetails()) {
        QVariantMap detailData;
        auto cocktail = m_cocktailRepository->getCocktailByUuid(detail.getCocktailUuid());
        if (cocktail) {
            detailData["name"] = cocktail->getName();
            detailData["quantity"] = detail.getQuantity();
            detailData["uuid"] = cocktail->getUuid();
            detailsList.append(detailData);
        }
    }
    return detailsList;
}

double CheckoutScreenController::totalPrice() const
{
    return m_currentSale.getTotalPrice();
}

double CheckoutScreenController::totalCupPawn() const
{
    return m_currentSale.getTotalCupPawn();
}

int CheckoutScreenController::returnedCups() const
{
    return m_currentSale.getReturnedCups();
}

QVariantList CheckoutScreenController::appliedDiscounts() const
{
    QVariantList discountsList;
    for (const auto &discount : m_currentSale.getAppliedDiscounts()) {
        if (!discount) {
            continue;
        }

        QVariantMap discountData;
        discountData["name"] = discount->getName();
        discountData["quantity"] = m_currentSale.getDiscountQuantity(discount->getUuid());
        discountData["uuid"] = discount->getUuid();
        discountsList.append(discountData);
    }
    return discountsList;
}

void CheckoutScreenController::confirmPayment(int paymentMethod)
{
    if (paymentMethod == static_cast<int>(PaymentMethod::Cash)) {
        m_currentSale.setPaymentMethod(PaymentMethod::Cash);

        // Kassenstand aktualisieren
        m_cashBalanceRepository->adjustBalance(m_currentSale.getTotalPrice(), "Sale completed");
    } else if (paymentMethod == static_cast<int>(PaymentMethod::Card)) {
        m_currentSale.setPaymentMethod(PaymentMethod::Card);
    }

    m_salesRepository->saveSale(m_currentSale);

    m_currentSale = Sale();
    m_currentSale.setPricePerCocktail(m_settingsRepository->getGeneralSettings()->getPricePerCocktail());
    m_currentSale.setCupPawn(m_settingsRepository->getGeneralSettings()->getCupPawn());

    emit currentSaleUpdated();
}

void CheckoutScreenController::returnCup()
{
    m_currentSale.incrementReturnedCups();
    emit currentSaleUpdated();
}

QVariantList CheckoutScreenController::availableDiscounts() const
{
    QVariantList discountsList;
    auto discounts = m_discountsRepository->getAllDiscounts();

    for (const auto &discount : discounts) {
        if (discount && discount->isActive()) {
            QVariantMap discountData;
            discountData["uuid"] = discount->getUuid();
            discountData["name"] = discount->getName();
            discountData["infoText"] = discount->getInfoText();
            discountsList.append(discountData);
        }
    }

    return discountsList;
}

void CheckoutScreenController::applyDiscount(const QString &discountUuid)
{
    const QSharedPointer<Discount> discount = m_discountsRepository->getDiscountByUuid(discountUuid);
    if (discount) {
        m_currentSale.incerementDiscountQuantity(discount);
        emit currentSaleUpdated();
    }
}

void CheckoutScreenController::decreaseDiscountQuantity(const QString &discountUuid)
{
    const QSharedPointer<Discount> discount = m_discountsRepository->getDiscountByUuid(discountUuid);
    if (!discount) {
        return;
    }

    m_currentSale.decrementDiscountQuantity(discount);
    emit currentSaleUpdated();
}
