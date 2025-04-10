#pragma once

#include "../entities/StatisticsData.h"
#include "../entities/StatisticsDataByTime.h"
#include "../entities/WeatherData.h"
#include "../repositories/SalesRepository.h"
#include "../repositories/WeatherRepository.h"

#include <QObject>
#include <QSharedPointer>
#include <QVariantList>


class StatisticsScreenController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList salesData READ salesData NOTIFY salesDataChanged)
    Q_PROPERTY(double totalRevenue READ getTotalRevenue NOTIFY salesDataChanged)
    Q_PROPERTY(int totalCocktailsSold READ getTotalCocktailsSold NOTIFY salesDataChanged)
    Q_PROPERTY(QString bestSellerCocktail READ getBestSellerCocktail NOTIFY salesDataChanged)
    Q_PROPERTY(QVariantList salesDataByTime READ salesDataByTime NOTIFY salesDataChanged)
    Q_PROPERTY(QVariantList weatherDataByTime READ weatherDataByTime NOTIFY weatherDataByTimeChanged)

public:
    explicit StatisticsScreenController(QSharedPointer<SalesRepository> salesRepository,
                                        QSharedPointer<WeatherRepository> weatherRepository,
                                        QObject *parent = nullptr);

    Q_INVOKABLE void fetchCocktailSales(const QString &startDate, const QString &endDate);

    QVariantList salesData() const;
    QVariantList salesDataByTime() const;
    QVariantList weatherDataByTime() const;

    double getTotalRevenue() const;
    int getTotalCocktailsSold() const;
    QString getBestSellerCocktail() const;

signals:
    void salesDataChanged();
    void invalidDateRange();
    void weatherDataByTimeChanged();

private:
    QSharedPointer<SalesRepository> m_salesRepository;
    QSharedPointer<WeatherRepository> m_weatherRepository;

    QList<StatisticsData> m_salesDataList;
    QList<StatisticsDataByTime> m_salesDataByTimeList;
    QList<WeatherData> m_weatherDataByTimeList;
};
