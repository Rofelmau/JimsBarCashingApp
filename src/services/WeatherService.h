#pragma once

#include "../entities/TemperatureCategory.h"
#include "../entities/WeatherCondition.h"

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class WeatherService : public QObject
{
    Q_OBJECT
public:
    explicit WeatherService(QObject *parent = nullptr);
    void fetchWeather(double latitude, double longitude);

signals:
    void weatherFetched(WeatherCondition condition, TemperatureCategory temperature);
    void fetchFailed(QString error);

private slots:
    void onWeatherReply(QNetworkReply *reply);

private:
    QNetworkAccessManager *networkManager;

    TemperatureCategory categorizeTemperature(double temp);
    WeatherCondition categorizeCondition(int weatherCode);
};
