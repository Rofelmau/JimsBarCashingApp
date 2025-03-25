#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class LocationService : public QObject
{
    Q_OBJECT
public:
    explicit LocationService(QObject *parent = nullptr);
    void fetchLocation();

signals:
    void locationFetched(QString city, double latitude, double longitude);
    void locationFetchFailed(QString error);

private slots:
    void onLocationReply(QNetworkReply *reply);

private:
    QNetworkAccessManager *manager;
};
