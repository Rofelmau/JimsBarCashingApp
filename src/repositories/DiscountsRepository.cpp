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
    query.prepare("SELECT uuid, name, type, value, cocktail_limit, active FROM Discounts");
    if (query.exec()) {
        while (query.next()) {
            discounts.append(QSharedPointer<Discount>::create(
                query.value("uuid").toString(),
                query.value("name").toString(),
                intToDiscountType(query.value("type").toInt()),
                query.value("value").toDouble(),
                query.value("cocktail_limit").toInt(),
                query.value("active").toBool()
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
    query.prepare("INSERT INTO Discounts (uuid, name, type, value, cocktail_limit, active) VALUES (?, ?, ?, ?, ?, ?)"); // UUID und Active hinzugefügt
    query.addBindValue(discount.getUuid());
    query.addBindValue(discount.getName());
    query.addBindValue(discountTypeToInt(discount.getType()));
    query.addBindValue(discount.getValue());
    query.addBindValue(discount.getCocktailLimit());
    query.addBindValue(discount.isActive());
    if (!query.exec()) {
        Logger::LogError("Failed to execute addDiscount query. Error:" + query.lastError().text().toStdString());
        return false;
    }
    return true;
}

bool DiscountsRepository::updateDiscount(const Discount &discount)
{
    QSqlQuery query(m_dbManager->database());
    query.prepare("UPDATE Discounts SET name = :name, type = :type, value = :value, cocktail_limit = :cocktail_limit WHERE uuid = :uuid");
    query.bindValue(":name", discount.getName());
    query.bindValue(":type", discountTypeToInt(discount.getType()));
    query.bindValue(":value", discount.getValue());
    query.bindValue(":cocktail_limit", discount.getCocktailLimit());
    query.bindValue(":uuid", discount.getUuid());
    if (!query.exec()) {
        Logger::LogError("Failed to execute updateDiscount query. Error:" + query.lastError().text().toStdString());
        return false;
    }
    return true;
}

bool DiscountsRepository::deleteDiscount(const QString &uuid)
{
    QSqlQuery query(m_dbManager->database());
    query.prepare("DELETE FROM Discounts WHERE uuid = :uuid");
    query.bindValue(":uuid", uuid);
    if (!query.exec()) {
        Logger::LogError("Failed to execute deleteDiscount query. Error:" + query.lastError().text().toStdString());
        return false;
    }
    return true;
}

bool DiscountsRepository::updateDiscountActiveStatus(const QString &uuid, bool active)
{
    QSqlQuery query(m_dbManager->database());
    query.prepare("UPDATE Discounts SET active = ? WHERE uuid = ?");
    query.addBindValue(active);
    query.addBindValue(uuid);

    if (!query.exec()) {
        Logger::LogError("Failed to execute updateDiscountActiveStatus query. Error:" + query.lastError().text().toStdString());
        return false;
    }

    return true;
}

QJsonArray DiscountsRepository::exportAsJson() const
{
    QJsonArray discountsArray;

    for (const QSharedPointer<Discount> &discount : getAllDiscounts()) {
        QJsonObject discountObject;
        discountObject["uuid"] = discount->getUuid();
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
    for (const QJsonValue &jsonValue : jsonArray) {
        if (!jsonValue.isObject()) {
            continue;
        }

        QJsonObject discountObject = jsonValue.toObject();
        QString uuid = discountObject["uuid"].toString();
        QString name = discountObject["name"].toString();
        DiscountType type = intToDiscountType(discountObject["type"].toInt());
        double discountValue = discountObject["value"].toDouble();
        int cocktailLimit = discountObject["cocktail_limit"].toInt();
        bool active = discountObject["active"].toBool();

        QSharedPointer<Discount> existingDiscount = getDiscountByUuid(uuid);
        if (existingDiscount) {
            Discount updatedDiscount(uuid, name, type, discountValue, cocktailLimit, active);
            updateDiscount(updatedDiscount);
        } else {
            Discount newDiscount(uuid, name, type, discountValue, cocktailLimit, active);
            addDiscount(newDiscount);
        }
    }
}

QSharedPointer<Discount> DiscountsRepository::getDiscountByUuid(const QString &uuid) const
{
    QSqlQuery query(m_dbManager->database());
    query.prepare("SELECT uuid, name, type, value, cocktail_limit, active FROM Discounts WHERE uuid = ?");
    query.addBindValue(uuid);

    if (query.exec() && query.next()) {
        return QSharedPointer<Discount>::create(
            query.value("uuid").toString(),
            query.value("name").toString(),
            intToDiscountType(query.value("type").toInt()),
            query.value("value").toDouble(),
            query.value("cocktail_limit").toInt(),
            query.value("active").toBool()
        );
    }

    return nullptr;
}
