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

void DiscountsConfigurationScreenController::addDiscount(const QString &name, const QString &type, double value, int cocktailLimit, bool reusable, bool combinable)
{
    DiscountType discountType = DiscountTypeHelper::fromString(type);
    auto discount = QSharedPointer<Discount>::create(-1, name, discountType, value, cocktailLimit, reusable, combinable);
    if (m_discountsRepository->addDiscount(*discount))
    {
        loadDiscounts();
    }
}

void DiscountsConfigurationScreenController::updateDiscount(int id, const QString &name, const QString &type, double value, int cocktailLimit, bool reusable, bool combinable)
{
    DiscountType discountType = DiscountTypeHelper::fromString(type);
    auto discount = QSharedPointer<Discount>::create(id, name, discountType, value, cocktailLimit, reusable, combinable);
    if (m_discountsRepository->updateDiscount(*discount))
    {
        loadDiscounts();
    }
}

void DiscountsConfigurationScreenController::deleteDiscount(int id)
{
    if (m_discountsRepository->deleteDiscount(id))
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
