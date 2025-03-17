#include "SalesRepository.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

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
    query.bindValue(":timestamp", sale.getTimestamp());
    query.bindValue(":payment_method", static_cast<int>(sale.getPaymentMethod()));
    query.bindValue(":price_per_cocktail", sale.getPricePerCocktail());
    query.bindValue(":total_price", sale.getTotalPrice());
    query.bindValue(":returned_cups", sale.getReturnedCups());
    query.exec();

    int saleId = query.lastInsertId().toInt();

    // Insert the sale details
    for (const SaleDetail &detail : sale.getDetails()) {
        query.prepare("INSERT INTO SalesDetails (sale_id, cocktail_id, quantity) "
                      "VALUES (:sale_id, :cocktail_id, :quantity)");
        query.bindValue(":sale_id", saleId);
        query.bindValue(":cocktail_id", detail.getCocktailId());
        query.bindValue(":quantity", detail.getQuantity());
        query.exec();
    }
}
