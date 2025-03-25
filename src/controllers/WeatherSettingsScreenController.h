#pragma once

#include "../repositories/WeatherRepository.h"

#include <QObject>
#include <QSharedPointer>

class WeatherSettingsScreenController : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool hasWeatherData READ hasWeatherData NOTIFY weatherDataChanged)
    Q_PROPERTY(QString lastWeatherCondition READ lastWeatherCondition NOTIFY weatherDataChanged)
    Q_PROPERTY(QString lastTemperature READ lastTemperature NOTIFY weatherDataChanged)

public:
    explicit WeatherSettingsScreenController(QSharedPointer<WeatherRepository> weatherRepository, QObject *parent = nullptr);

    bool hasWeatherData() const;
    QString lastWeatherCondition() const;
    QString lastTemperature() const;

    Q_INVOKABLE void fetchLatestWeather();

signals:
    void weatherDataChanged();

private:
    QSharedPointer<WeatherRepository> m_weatherRepository;
    bool m_hasWeatherData{false};
    QString m_lastWeatherCondition;
    QString m_lastTemperature;
};
