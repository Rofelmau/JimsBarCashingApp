#pragma once

#include <QObject>
#include <QSharedPointer>
#include <QVariantList>
#include "../repositories/SalesRepository.h"

class StatisticsScreenController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList salesData READ salesData NOTIFY salesDataChanged)
    Q_PROPERTY(double totalRevenue READ getTotalRevenue NOTIFY salesDataChanged)
    Q_PROPERTY(int totalCocktailsSold READ getTotalCocktailsSold NOTIFY salesDataChanged)
    Q_PROPERTY(QString bestSellerCocktail READ getBestSellerCocktail NOTIFY salesDataChanged)
    Q_PROPERTY(QVariantList salesDataByTime READ salesDataByTime NOTIFY salesDataChanged)

public:
    explicit StatisticsScreenController(QSharedPointer<SalesRepository> salesRepository, QObject *parent = nullptr);

    Q_INVOKABLE void fetchCocktailSales(const QString &startDate, const QString &endDate);

    QVariantList salesData() const;
    QVariantList salesDataByTime() const;

    double getTotalRevenue() const;
    int getTotalCocktailsSold() const;
    QString getBestSellerCocktail() const;

signals:
    void salesDataChanged();
    void invalidDateRange();

private:
    QSharedPointer<SalesRepository> m_salesRepository;
    QList<SaleData> m_salesDataList;
    QList<SaleDataByTime> m_salesDataByTimeList;
};
