#include "CheckoutScreenController.h"

#include "../entities/Sale.h"

CheckoutScreenController::CheckoutScreenController(QSharedPointer<SettingsRepository> settingsRepo
                                                    , QSharedPointer<CocktailRepository> cocktailRepo
                                                    , QSharedPointer<SalesRepository> salesRepo
                                                    , QObject *parent)
    : QObject(parent)
    , m_settingsRepository(settingsRepo)
    , m_cocktailRepository(cocktailRepo)
    , m_salesRepository(salesRepo)
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
            cocktailData["id"] = cocktail->getId();
            cocktailData["name"] = cocktail->getName();
            cocktailsList.append(cocktailData);
        }
    }

    return cocktailsList;
}

void CheckoutScreenController::cocktailSelected(int cocktailId)
{
    auto cocktail = m_cocktailRepository->getCocktailById(cocktailId);
    if (cocktail) {
        m_currentSale.incrementQuantity(cocktailId);

        emit currentSaleUpdated();
    }
}

void CheckoutScreenController::decreaseQuantity(int cocktailId)
{
    m_currentSale.decrementQuantity(cocktailId);
    emit currentSaleUpdated();
}

QVariantList CheckoutScreenController::currentSaleDetails() const
{
    QVariantList detailsList;
    for (const SaleDetail &detail : m_currentSale.getDetails()) {
        QVariantMap detailData;
        auto cocktail = m_cocktailRepository->getCocktailById(detail.getCocktailId());
        if (cocktail) {
            detailData["name"] = cocktail->getName();
            detailData["quantity"] = detail.getQuantity();
            detailData["id"] = cocktail->getId();
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

void CheckoutScreenController::confirmPayment(int paymentMethod)
{
    if (paymentMethod == static_cast<int>(PaymentMethod::Cash)) {
        m_currentSale.setPaymentMethod(PaymentMethod::Cash);
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