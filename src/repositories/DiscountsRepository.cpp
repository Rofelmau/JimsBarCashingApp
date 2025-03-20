#include "DiscountsRepository.h"

#include "DiscountType.h"

#include <QSqlQuery>
#include <QVariant>

#include <stdexcept>

namespace {
    int discountTypeToInt(DiscountType type) {
        return static_cast<int>(type);
    }

    DiscountType intToDiscountType(int type) {
        if (!DiscountTypeHelper::isValidDiscountType(type)) {
            throw std::invalid_argument("Invalid DiscountType integer");
        }
        return static_cast<DiscountType>(type);
    }
}

DiscountsRepository::DiscountsRepository(QSharedPointer<DatabaseManager> dbManager)
    : m_dbManager(dbManager)
{
}

QList<QSharedPointer<Discount>> DiscountsRepository::getAllDiscounts()
{
    QList<QSharedPointer<Discount>> discounts;
    QSqlQuery query(m_dbManager->database());
    query.prepare("SELECT id, name, type, value, cocktail_limit, reusable, combinable FROM Discounts");
    if (query.exec()) {
        while (query.next()) {
            discounts.append(QSharedPointer<Discount>::create(
                query.value(0).toInt(),
                query.value(1).toString(),
                intToDiscountType(query.value(2).toInt()),
                query.value(3).toDouble(),
                query.value(4).toInt(),
                query.value(5).toBool(),
                query.value(6).toBool()
            ));
        }
    }
    return discounts;
}

bool DiscountsRepository::addDiscount(const Discount &discount)
{
    QSqlQuery query(m_dbManager->database());
    query.prepare("INSERT INTO Discounts (name, type, value, cocktail_limit, reusable, combinable) VALUES (?, ?, ?, ?, ?, ?)");
    query.addBindValue(discount.getName());
    query.addBindValue(discountTypeToInt(discount.getType()));
    query.addBindValue(discount.getValue());
    query.addBindValue(discount.getCocktailLimit());
    query.addBindValue(discount.isReusable());
    query.addBindValue(discount.isCombinable());
    return query.exec();
}

bool DiscountsRepository::updateDiscount(const Discount &discount)
{
    QSqlQuery query(m_dbManager->database());
    query.prepare("UPDATE Discounts SET name = ?, type = ?, value = ?, cocktail_limit = ?, reusable = ?, combinable = ? WHERE id = ?");
    query.addBindValue(discount.getName());
    query.addBindValue(discountTypeToInt(discount.getType()));    query.addBindValue(discount.getValue());
    query.addBindValue(discount.getCocktailLimit());
    query.addBindValue(discount.isReusable());
    query.addBindValue(discount.isCombinable());
    query.addBindValue(discount.getId());
    return query.exec();
}

bool DiscountsRepository::deleteDiscount(int discountId)
{
    QSqlQuery query(m_dbManager->database());
    query.prepare("DELETE FROM Discounts WHERE id = ?");
    query.addBindValue(discountId);
    return query.exec();
}
