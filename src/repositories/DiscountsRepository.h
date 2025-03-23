#pragma once

#include "Discount.h"
#include "DatabaseManager.h"

#include <QList>
#include <QSharedPointer>

class DiscountsRepository {
public:
    explicit DiscountsRepository(QSharedPointer<DatabaseManager> dbManager);

    QList<QSharedPointer<Discount>> getAllDiscounts();
    bool addDiscount(const Discount &discount);
    bool updateDiscount(const Discount &discount);
    bool deleteDiscount(int discountId);

    QSharedPointer<Discount> getDiscountById(int discountId);

private:
    QSharedPointer<DatabaseManager> m_dbManager;
};
