#pragma once

#include "../DatabaseManager.h"
#include "../entities/Sale.h"
#include "../entities/StatisticsData.h"
#include "../entities/StatisticsDataByTime.h"

#include <QObject>
#include <QSharedPointer>
#include <QList>
#include <QString>

class SalesRepository : public QObject
{
    Q_OBJECT
public:
    explicit SalesRepository(QSharedPointer<DatabaseManager> dbManager, QObject *parent = nullptr);

    void saveSale(const Sale &sale);
    QList<StatisticsData> getSalesData(const QString &startDate, const QString &endDate);
    QList<StatisticsDataByTime> getSalesDataByTime(const QString &startDate, const QString &endDate);

private:
    QSharedPointer<DatabaseManager> m_databaseManager;
};
