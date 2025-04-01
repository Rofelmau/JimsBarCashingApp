#pragma once

#include "Discount.h"
#include "DatabaseManager.h"

#include <QJsonArray>
#include <QList>
#include <QSharedPointer>

class DiscountsRepository {
public:
    explicit DiscountsRepository(QSharedPointer<DatabaseManager> dbManager);

    QList<QSharedPointer<Discount>> getAllDiscounts() const;
    bool addDiscount(const Discount &discount);
    bool updateDiscount(const Discount &discount);
    bool deleteDiscount(const QString &uuid);
    QSharedPointer<Discount> getDiscountByUuid(const QString &uuid) const;
    bool updateDiscountActiveStatus(const QString &uuid, bool active);

    QJsonArray exportAsJson() const;
    void import(const QJsonArray &jsonArray);

private:
    QSharedPointer<DatabaseManager> m_dbManager;
};
