#include "DiscountsRepository.h"

#include "../Logger.h"

#include "DiscountType.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QSqlError>
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

QList<QSharedPointer<Discount>> DiscountsRepository::getAllDiscounts() const
{
    QList<QSharedPointer<Discount>> discounts;
    QSqlQuery query(m_dbManager->database());
    query.prepare("SELECT id, name, type, value, cocktail_limit, active FROM Discounts");
    if (query.exec()) {
        while (query.next()) {
            discounts.append(QSharedPointer<Discount>::create(
                query.value(0).toInt(),
                query.value(1).toString(),
                intToDiscountType(query.value(2).toInt()),
                query.value(3).toDouble(),
                query.value(4).toInt(),
                query.value(5).toBool()
            ));
        }
    } else {
        Logger::LogError("Failed to execute getAllDiscounts query. Error:" + query.lastError().text().toStdString());
    }
    return discounts;
}

bool DiscountsRepository::addDiscount(const Discount &discount)
{
    QSqlQuery query(m_dbManager->database());
    query.prepare("INSERT INTO Discounts (name, type, value, cocktail_limit) VALUES (?, ?, ?, ?)");
    query.addBindValue(discount.getName());
    query.addBindValue(discountTypeToInt(discount.getType()));
    query.addBindValue(discount.getValue());
    query.addBindValue(discount.getCocktailLimit());
    if (!query.exec()) {
        Logger::LogError("Failed to execute addDiscount query. Error:" + query.lastError().text().toStdString());
        return false;
    }
    return true;
}

bool DiscountsRepository::updateDiscount(const Discount &discount)
{
    QSqlQuery query(m_dbManager->database());
    query.prepare("UPDATE Discounts SET name = ?, type = ?, value = ?, cocktail_limit = ? WHERE id = ?");
    query.addBindValue(discount.getName());
    query.addBindValue(discountTypeToInt(discount.getType()));
    query.addBindValue(discount.getValue());
    query.addBindValue(discount.getCocktailLimit());
    query.addBindValue(discount.getId());
    if (!query.exec()) {
        Logger::LogError("Failed to execute updateDiscount query. Error:" + query.lastError().text().toStdString());
        return false;
    }
    return true;
}

bool DiscountsRepository::deleteDiscount(int discountId)
{
    QSqlQuery query(m_dbManager->database());
    query.prepare("DELETE FROM Discounts WHERE id = ?");
    query.addBindValue(discountId);
    if (!query.exec()) {
        Logger::LogError("Failed to execute deleteDiscount query. Error:" + query.lastError().text().toStdString());
        return false;
    }
    return true;
}

QSharedPointer<Discount> DiscountsRepository::getDiscountById(int discountId)
{
    QSqlQuery query(m_dbManager->database());
    query.prepare("SELECT name, type, value, cocktail_limit FROM Discounts WHERE id = ?");
    query.addBindValue(discountId);
    if (query.exec()) {
        if (query.next()) {
            return QSharedPointer<Discount>::create(
                discountId,
                query.value(0).toString(),
                intToDiscountType(query.value(1).toInt()),
                query.value(2).toDouble(),
                query.value(3).toInt()
            );
        } else {
            Logger::LogWarn("No discount found with ID:" + discountId);
        }
    } else {
        Logger::LogError("Failed to execute getDiscountById query. Error:" + query.lastError().text().toStdString());
    }
    return nullptr;
}

bool DiscountsRepository::updateDiscountActiveStatus(int discountId, bool active)
{
    QSqlQuery query(m_dbManager->database());
    query.prepare("UPDATE Discounts SET active = ? WHERE id = ?");
    query.addBindValue(active);
    query.addBindValue(discountId);

    if (!query.exec())
    {
        return false;
    }

    return true;
}

QJsonArray DiscountsRepository::exportAsJson() const
{
    QJsonArray discountsArray;

    for (const QSharedPointer<Discount> &discount : getAllDiscounts()) {
        QJsonObject discountObject;
        discountObject["id"] = discount->getId();
        discountObject["name"] = discount->getName();
        discountObject["type"] = static_cast<int>(discount->getType());
        discountObject["value"] = discount->getValue();
        discountObject["cocktail_limit"] = discount->getCocktailLimit();
        discountObject["active"] = discount->isActive();

        discountsArray.append(discountObject);
    }

    return discountsArray;
}

void DiscountsRepository::import(const QJsonArray &jsonArray)
{
    for (const QJsonValue &value : jsonArray) {
        if (!value.isObject()) {
            continue;
        }

        QJsonObject discountObject = value.toObject();
        Discount discount(
            discountObject["id"].toInt(),
            discountObject["name"].toString(),
            intToDiscountType(discountObject["type"].toInt()),
            discountObject["value"].toDouble(),
            discountObject["cocktail_limit"].toInt(),
            discountObject["active"].toBool()
        );

        addDiscount(discount);
    }
}
