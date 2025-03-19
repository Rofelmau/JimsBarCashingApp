#pragma once

#include "../DatabaseManager.h"
#include "../entities/Sale.h"

#include <QObject>
#include <QSharedPointer>
#include <QList>
#include <QString>

struct SaleData {
    QString cocktailName;
    int quantitySold;
    double pricePerCocktail;
};

struct SaleDataByTime {
    QString timePeriod;
    int quantitySold;
};

class SalesRepository : public QObject
{
    Q_OBJECT
public:
    explicit SalesRepository(QSharedPointer<DatabaseManager> dbManager, QObject *parent = nullptr);

    void saveSale(const Sale &sale);
    QList<SaleData> getSalesData(const QString &startDate, const QString &endDate);
    QList<SaleDataByTime> getSalesDataByTime(const QString &startDate, const QString &endDate);

private:
    QSharedPointer<DatabaseManager> m_databaseManager;
};
