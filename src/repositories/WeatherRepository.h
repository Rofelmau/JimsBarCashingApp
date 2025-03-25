#pragma once

#include "../DatabaseManager.h"
#include "../entities/WeatherData.h"

#include <QObject>
#include <QSharedPointer>
#include <QSqlDatabase>

class WeatherRepository : public QObject {
    Q_OBJECT
public:
    explicit WeatherRepository(QSharedPointer<DatabaseManager> dbManager);

    bool saveWeatherData(const WeatherData &data);
    QList<WeatherData> fetchWeatherData();

signals:
    void weatherDataUpdated();

private:
    QSharedPointer<DatabaseManager> m_dbManager;
};
