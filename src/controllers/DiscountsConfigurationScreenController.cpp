#include "DiscountsConfigurationScreenController.h"

#include "DiscountType.h"

DiscountsConfigurationScreenController::DiscountsConfigurationScreenController(QSharedPointer<DiscountsRepository> discountsRepository, QObject *parent)
    : QObject(parent)
    , m_discountsRepository(discountsRepository)
    , m_discountModel(new DiscountModel(this))
{
    loadDiscounts();
}

DiscountModel *DiscountsConfigurationScreenController::discountModel() const
{
    return m_discountModel.data();
}

void DiscountsConfigurationScreenController::addDiscount(const QString &name, const int type, double value, int cocktailLimit)
{
    DiscountType discountType = static_cast<DiscountType>(type);
    const QString& uuid = QUuid::createUuid().toString();
    auto discount = QSharedPointer<Discount>::create(uuid, name, discountType, value, cocktailLimit, true);
    if (m_discountsRepository->addDiscount(*discount))
    {
        loadDiscounts();
    }
}

void DiscountsConfigurationScreenController::updateDiscount(const QString &uuid, const QString &name, const int type, double value, int cocktailLimit)
{
    DiscountType discountType = static_cast<DiscountType>(type);
    auto discount = QSharedPointer<Discount>::create(uuid, name, discountType, value, cocktailLimit, true);
    if (m_discountsRepository->updateDiscount(*discount)) {
        loadDiscounts();
    }
}

void DiscountsConfigurationScreenController::deleteDiscount(const QString &uuid)
{
    if (m_discountsRepository->deleteDiscount(uuid)) {
        loadDiscounts();
    }
}

void DiscountsConfigurationScreenController::toggleDiscountActiveStatus(const QString &uuid, bool active)
{
    if (m_discountsRepository->updateDiscountActiveStatus(uuid, active))
    {
        loadDiscounts();
    }
}

void DiscountsConfigurationScreenController::loadDiscounts()
{
    auto discounts = m_discountsRepository->getAllDiscounts();
    m_discountModel->setDiscounts(discounts);
    emit discountsChanged();
}
