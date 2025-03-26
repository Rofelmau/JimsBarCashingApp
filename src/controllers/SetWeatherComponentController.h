#pragma once

#include "../entities/TemperatureCategory.h"
#include "../entities/WeatherCondition.h"
#include "../repositories/WeatherRepository.h"
#include "../services/LocationService.h"
#include "../services/WeatherService.h"

#include <QObject>
#include <QSharedPointer>

class SetWeatherComponentController : public QObject {
    Q_OBJECT
    Q_PROPERTY(WeatherCondition weatherCondition READ weatherCondition NOTIFY weatherDataChanged)
    Q_PROPERTY(TemperatureCategory temperatureCategory READ temperatureCategory NOTIFY weatherDataChanged)
    Q_PROPERTY(bool isLoading READ isLoading NOTIFY loadingStateChanged)

public:
    explicit SetWeatherComponentController(QSharedPointer<WeatherService> weatherService,
                                            QSharedPointer<LocationService> locationService,
                                            QSharedPointer<WeatherRepository> weatherRepository,
                                            QObject *parent = nullptr);

    WeatherCondition weatherCondition() const;
    TemperatureCategory temperatureCategory() const;
    bool isLoading() const;

    Q_INVOKABLE void fetchWeatherForLocation();
    Q_INVOKABLE void saveWeatherData();
    Q_INVOKABLE void setManualWeatherCondition(WeatherCondition condition);
    Q_INVOKABLE void setManualTemperatureCategory(TemperatureCategory category);

signals:
    void weatherDataChanged();
    void loadingStateChanged();
    void fetchFailed(const QString &errorMessage);
    void weatherDataSaved();

private slots:
    void onWeatherFetched(WeatherCondition condition, TemperatureCategory temperature);

private:
    QSharedPointer<WeatherService> m_weatherService;
    QSharedPointer<LocationService> m_locationService;
    QSharedPointer<WeatherRepository> m_weatherRepository;
    WeatherCondition m_weatherCondition{WeatherCondition::Sunny};
    TemperatureCategory m_temperatureCategory{TemperatureCategory::Pleasant};
    bool m_isLoading{false};
};
