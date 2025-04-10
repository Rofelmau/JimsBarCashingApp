#include "SalesRepository.h"

#include "../Logger.h"
#include "../entities/StatisticsData.h"
#include "../entities/StatisticsDataByTime.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>
#include <QTimeZone>

SalesRepository::SalesRepository(QSharedPointer<DatabaseManager> dbManager, QObject *parent)
    : QObject(parent)
    , m_databaseManager(dbManager)
{
}

void SalesRepository::saveSale(const Sale &sale)
{
    QSqlDatabase db = m_databaseManager->database();
    if (!db.isOpen()) {
        Logger::LogError("Database is not open. Cannot save sale.");
        return;
    }

    QSqlQuery query(db);

    query.prepare("INSERT INTO Sales (timestamp, payment_method, price_per_cocktail, total_price, returned_cups) "
                  "VALUES (:timestamp, :payment_method, :price_per_cocktail, :total_price, :returned_cups)");
    query.bindValue(":timestamp", sale.getTimestamp().toUTC().toString(Qt::ISODate)); // Save in UTC ISO 8601 format
    query.bindValue(":payment_method", static_cast<int>(sale.getPaymentMethod()));
    query.bindValue(":price_per_cocktail", sale.getPricePerCocktail());
    query.bindValue(":total_price", sale.getTotalPrice());
    query.bindValue(":returned_cups", sale.getReturnedCups());
    if (!query.exec()) {
        Logger::LogError("Failed to save sale: " + query.lastError().text().toStdString());
        return;
    }

    int saleId = query.lastInsertId().toInt();

    for (const SaleDetail &detail : sale.getDetails()) {
        QSqlQuery detailQuery(db);
        detailQuery.prepare(R"(
            INSERT INTO SalesDetails (sale_id, cocktail_uuid, quantity)
            VALUES (:sale_id, :cocktail_uuid, :quantity)
        )");
        detailQuery.bindValue(":sale_id", saleId);
        detailQuery.bindValue(":cocktail_uuid", detail.getCocktailUuid());
        detailQuery.bindValue(":quantity", detail.getQuantity());
        if (!detailQuery.exec()) {
            Logger::LogError("Failed to save sale detail: " + detailQuery.lastError().text().toStdString());
        }
    }

    for (const auto &discount : sale.getAppliedDiscounts()) {
        if (!discount) {
            continue;
        }
        query.prepare(R"(
            INSERT INTO SalesDiscounts (sale_id, discount_uuid, quantity)
            VALUES (:sale_id, :discount_uuid, :quantity)
        )");
        query.bindValue(":sale_id", saleId);
        query.bindValue(":discount_uuid", discount->getUuid());
        query.bindValue(":quantity", sale.getDiscountQuantity(discount->getUuid()));
        if (!query.exec()) {
            Logger::LogError("Failed to save sale discount: " + query.lastError().text().toStdString());
        }
    }
}

QList<StatisticsData> SalesRepository::getSalesData(const QString &startDate, const QString &endDate)
{
    QList<StatisticsData> salesData;
    QSqlDatabase db = m_databaseManager->database();
    QSqlQuery query(db);

    if (startDate.isEmpty() || endDate.isEmpty()) {
        Logger::LogError("Invalid date range: startDate or endDate is empty.");
        return salesData;
    }

    QDate startQDate = QDate::fromString(startDate, "yyyy-MM-dd");
    QDate endQDate = QDate::fromString(endDate, "yyyy-MM-dd");

    QDateTime startDateTime = startQDate.startOfDay(Qt::UTC);
    QDateTime endDateTime = endQDate.endOfDay(Qt::UTC);

    query.prepare(R"(
        SELECT Cocktails.name AS cocktailName, 
               SUM(SalesDetails.quantity) AS quantitySold,
               Sales.price_per_cocktail AS pricePerCocktail,
               SUM(Sales.total_price) AS totalPrice
        FROM Sales
        INNER JOIN SalesDetails ON Sales.id = SalesDetails.sale_id
        INNER JOIN Cocktails ON SalesDetails.cocktail_uuid = Cocktails.uuid
        WHERE Sales.timestamp BETWEEN :startDate AND :endDate
        GROUP BY Cocktails.name, Sales.price_per_cocktail
        ORDER BY quantitySold DESC
    )");

    query.bindValue(":startDate", startDateTime.toString(Qt::ISODate));
    query.bindValue(":endDate", endDateTime.toString(Qt::ISODate));

    if (!query.exec()) {
        Logger::LogError("Failed to fetch sales data: " + query.lastError().text().toStdString());
        return salesData;
    }

    while (query.next()) {
        StatisticsData data;
        data.setCocktailName(query.value("cocktailName").toString());
        data.setQuantitySold(query.value("quantitySold").toInt());
        data.setPricePerCocktail(query.value("pricePerCocktail").toDouble());
        data.setTotalPrice(query.value("totalPrice").toDouble());
        salesData.append(data);
    }

    return salesData;
}

QList<StatisticsDataByTime> SalesRepository::getSalesDataByTime(const QString &startDate, const QString &endDate)
{
    QList<StatisticsDataByTime> salesDataByTime;
    QSqlDatabase db = m_databaseManager->database();
    QSqlQuery query(db);

    if (startDate.isEmpty() || endDate.isEmpty()) {
        Logger::LogError("Invalid date range: startDate or endDate is empty.");
        return salesDataByTime;
    }

    query.prepare(R"(
        SELECT strftime('%Y-%m-%d %H:00', Sales.timestamp) AS timePeriod,
               SUM(SalesDetails.quantity) AS quantitySold
        FROM Sales
        INNER JOIN SalesDetails ON Sales.id = SalesDetails.sale_id
        WHERE Sales.timestamp BETWEEN :startDate AND :endDate
        GROUP BY timePeriod
        ORDER BY timePeriod ASC
    )");

    QDate startQDate = QDate::fromString(startDate, "yyyy-MM-dd");
    QDate endQDate = QDate::fromString(endDate, "yyyy-MM-dd");

    QDateTime startDateTime = startQDate.startOfDay(Qt::UTC);
    QDateTime endDateTime = endQDate.endOfDay(Qt::UTC);

    query.bindValue(":startDate", startDateTime.toString(Qt::ISODate));
    query.bindValue(":endDate", endDateTime.toString(Qt::ISODate));

    if (!query.exec()) {
        Logger::LogError("Failed to fetch sales data by time: " + query.lastError().text().toStdString());
        return salesDataByTime;
    }

    while (query.next()) {
        StatisticsDataByTime data;
        data.setTimePeriod(query.value("timePeriod").toString());
        data.setQuantitySold(query.value("quantitySold").toInt());
        salesDataByTime.append(data);
    }

    return salesDataByTime;
}
