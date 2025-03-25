#include "LocationService.h"

#include "../Logger.h"

#include <QJsonDocument>
#include <QJsonObject>

LocationService::LocationService(QObject *parent)
    : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &LocationService::onLocationReply);
}

void LocationService::fetchLocation()
{
    Logger::LogInfo("Fetching location...");
    QUrl url("https://ipinfo.io/json");  // Standortbestimmung über IP
    QNetworkRequest request(url);
    manager->get(request);
}

void LocationService::onLocationReply(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        Logger::LogWarn("Failed to fetch location: " + reply->errorString().toStdString());
        emit locationFetchFailed(reply->errorString());
        reply->deleteLater();
        return;
    }

    QByteArray response = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(response);
    QJsonObject json = doc.object();

    QString city = json["city"].toString();
    QString loc = json["loc"].toString();  // Format: "lat,lon"
    QStringList coords = loc.split(",");

    if (coords.size() != 2) {
        Logger::LogWarn("Coordinates not found in location response.");
        emit locationFetchFailed("Koordinaten nicht gefunden");
        return;
    }

    double latitude = coords[0].toDouble();
    double longitude = coords[1].toDouble();

    Logger::LogInfo("Location fetched: City: " + city.toStdString() +
                    ", Latitude: " + std::to_string(latitude) +
                    ", Longitude: " + std::to_string(longitude));
    emit locationFetched(city, latitude, longitude);

    reply->deleteLater();
}
