#include "WeatherService.h"

#include "../Logger.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkRequest>

WeatherService::WeatherService(QObject *parent)
    : QObject(parent)
{
    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished, this, &WeatherService::onWeatherReply);
}

void WeatherService::fetchWeather(double latitude, double longitude)
{
    QUrl url(QString("https://api.open-meteo.com/v1/forecast?latitude=%1&longitude=%2&current_weather=true")
                 .arg(latitude)
                 .arg(longitude));

    QNetworkRequest request(url);
    networkManager->get(request);
}

void WeatherService::onWeatherReply(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        Logger::LogWarn("Fehler beim Abrufen des Wetters:" + reply->errorString().toStdString());
        emit fetchFailed(reply->errorString());
        reply->deleteLater();
        return;
    }

    QByteArray response = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(response);
    QJsonObject json = doc.object();

    if (!json.contains("current_weather")) {
        emit fetchFailed("Keine Wetterdaten verfügbar");
        return;
    }

    QJsonObject currentWeather = json["current_weather"].toObject();
    double temperature = currentWeather["temperature"].toDouble();
    int weatherCode = currentWeather["weathercode"].toInt();

    TemperatureCategory temperatureCategory = categorizeTemperature(temperature);
    WeatherCondition conditionCategory = categorizeCondition(weatherCode);

    emit weatherFetched(conditionCategory, temperatureCategory);

    reply->deleteLater();
}

TemperatureCategory WeatherService::categorizeTemperature(double temp)
{
    if (temp < 0) return TemperatureCategory::VeryCold;
    if (temp < 10) return TemperatureCategory::Cool;
    if (temp < 20) return TemperatureCategory::Pleasant;
    if (temp < 30) return TemperatureCategory::Warm;
    return TemperatureCategory::Hot;
}

WeatherCondition WeatherService::categorizeCondition(int weatherCode)
{
    if (weatherCode == 0) return WeatherCondition::Sunny;
    if (weatherCode == 1 || weatherCode == 2) return WeatherCondition::Cloudy;
    if (weatherCode == 3) return WeatherCondition::Cloudy;
    if (weatherCode >= 61 && weatherCode <= 65) return WeatherCondition::Rainy;
    if (weatherCode >= 95) return WeatherCondition::Stormy;
    if (weatherCode >= 71 && weatherCode <= 75) return WeatherCondition::Snowy;
    return WeatherCondition::Cloudy;
}
