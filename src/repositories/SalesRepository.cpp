#include "SalesRepository.h"

#include "../Logger.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <QTimeZone>

SalesRepository::SalesRepository(QSharedPointer<DatabaseManager> dbManager, QObject *parent)
    : QObject(parent)
    , m_databaseManager(dbManager)
{
}

void SalesRepository::saveSale(const Sale &sale)
{
    QSqlDatabase db = m_databaseManager->database();
    QSqlQuery query(db);

    // Insert the sale
    query.prepare("INSERT INTO Sales (timestamp, payment_method, price_per_cocktail, total_price, returned_cups) "
                  "VALUES (:timestamp, :payment_method, :price_per_cocktail, :total_price, :returned_cups)");
    query.bindValue(":timestamp", sale.getTimestamp().toString(Qt::ISODate));
    query.bindValue(":payment_method", static_cast<int>(sale.getPaymentMethod()));
    query.bindValue(":price_per_cocktail", sale.getPricePerCocktail());
    query.bindValue(":total_price", sale.getTotalPrice());
    query.bindValue(":returned_cups", sale.getReturnedCups());
    if (!query.exec()) {
        Logger::LogError("Failed to save sale: " + query.lastError().text().toStdString());
    }

    int saleId = query.lastInsertId().toInt();

    for (const SaleDetail &detail : sale.getDetails()) {
        query.prepare("INSERT INTO SalesDetails (sale_id, cocktail_id, quantity) "
                      "VALUES (:sale_id, :cocktail_id, :quantity)");
        query.bindValue(":sale_id", saleId);
        query.bindValue(":cocktail_id", detail.getCocktailId());
        query.bindValue(":quantity", detail.getQuantity());
        if (!query.exec()) {
            Logger::LogError("Failed to save sale detail: " + query.lastError().text().toStdString());
        }
    }
}

QList<SaleData> SalesRepository::getSalesData(const QString &startDate, const QString &endDate)
{
    QList<SaleData> salesData;

    QSqlDatabase db = m_databaseManager->database();
    QSqlQuery query(db);

    if (startDate.isEmpty() || endDate.isEmpty()) {
        Logger::LogError("Invalid date range: startDate or endDate is empty.");
        return salesData;
    }

    // Konvertiere in QDate
    QDate startQDate = QDate::fromString(startDate, "yyyy-MM-dd");
    QDate endQDate = QDate::fromString(endDate, "yyyy-MM-dd");

    if (!startQDate.isValid() || !endQDate.isValid()) {
        Logger::LogError("Invalid date format: startDate or endDate is not in 'yyyy-MM-dd' format.");
        return salesData;
    }

    // Lokale Zeit erzeugen
    QDateTime startDateTimeLocal(startQDate, QTime(0, 0, 0), QTimeZone::systemTimeZone());
    QDateTime endDateTimeLocal(endQDate, QTime(23, 59, 59), QTimeZone::systemTimeZone());

    // In UTC umwandeln, um mit der Datenbank übereinzustimmen
    QDateTime startDateTimeUTC = startDateTimeLocal.toUTC();
    QDateTime endDateTimeUTC = endDateTimeLocal.toUTC();

    QString startDateString = startDateTimeUTC.toString("yyyy-MM-dd HH:mm:ss");
    QString endDateString = endDateTimeUTC.toString("yyyy-MM-dd HH:mm:ss");

    query.prepare(R"(
        SELECT Cocktails.name AS cocktailName, 
               SUM(SalesDetails.quantity) AS quantitySold,
               Sales.price_per_cocktail AS pricePerCocktail
        FROM Sales
        INNER JOIN SalesDetails ON Sales.id = SalesDetails.sale_id
        INNER JOIN Cocktails ON SalesDetails.cocktail_id = Cocktails.id
        WHERE Sales.timestamp BETWEEN :startDate AND :endDate
        GROUP BY Cocktails.name, Sales.price_per_cocktail
        ORDER BY quantitySold DESC
    )");

    query.bindValue(":startDate", startDateString);
    query.bindValue(":endDate", endDateString);

    if (!query.exec()) {
        Logger::LogError("Failed to fetch sales data: " + query.lastError().text().toStdString());
        return salesData;
    }

    while (query.next()) {
        SaleData data;
        data.cocktailName = query.value("cocktailName").toString();
        data.quantitySold = query.value("quantitySold").toInt();
        data.pricePerCocktail = query.value("pricePerCocktail").toDouble();
        salesData.append(data);
    }

    return salesData;
}

QList<SaleDataByTime> SalesRepository::getSalesDataByTime(const QString &startDate, const QString &endDate)
{
    QList<SaleDataByTime> salesDataByTime;

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

    query.bindValue(":startDate", startDate);
    query.bindValue(":endDate", endDate);

    if (!query.exec()) {
        Logger::LogError("Failed to fetch sales data by time: " + query.lastError().text().toStdString());
        return salesDataByTime;
    }

    while (query.next()) {
        SaleDataByTime data;
        data.timePeriod = query.value("timePeriod").toString();
        data.quantitySold = query.value("quantitySold").toInt();
        salesDataByTime.append(data);
    }

    return salesDataByTime;
}
