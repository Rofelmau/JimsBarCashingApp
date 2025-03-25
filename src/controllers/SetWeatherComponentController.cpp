#include "SetWeatherComponentController.h"

#include "../Logger.h"
#include "../entities/WeatherData.h"

SetWeatherComponentController::SetWeatherComponentController(QSharedPointer<WeatherService> weatherService, QSharedPointer<LocationService> locationService, QSharedPointer<WeatherRepository> weatherRepository, QObject *parent)
    : QObject(parent)
    , m_weatherService(weatherService)
    , m_locationService(locationService)
    , m_weatherRepository(weatherRepository)
{
    connect(m_weatherService.data(), &WeatherService::weatherFetched, this, &SetWeatherComponentController::onWeatherFetched);
    connect(m_weatherService.data(), &WeatherService::fetchFailed, this, [this](QString error) {
        Logger::LogWarn("Weather fetch failed: " + error.toStdString());
        m_isLoading = false;
        emit loadingStateChanged();
        emit fetchFailed("Wetter konnte nicht abgerufen werden: " + error);
    });

    connect(m_locationService.data(), &LocationService::locationFetched, this, [this](QString city, double latitude, double longitude)
    {
        Logger::LogInfo("Location fetched: City: " + city.toStdString() +
                        ", Latitude: " + std::to_string(latitude) +
                        ", Longitude: " + std::to_string(longitude));
        m_weatherService->fetchWeather(latitude, longitude);
    });

    connect(m_locationService.data(), &LocationService::locationFetchFailed, this, [this](QString error)
    {
        Logger::LogWarn("Failed to fetch location: " + error.toStdString());
        m_isLoading = false;
        emit loadingStateChanged();
        emit fetchFailed("Standort konnte nicht abgerufen werden: " + error);
    });
}

WeatherCondition SetWeatherComponentController::weatherCondition() const
{
    return m_weatherCondition;
}

TemperatureCategory SetWeatherComponentController::temperatureCategory() const
{
    return m_temperatureCategory;
}

bool SetWeatherComponentController::isLoading() const
{
    return m_isLoading;
}

void SetWeatherComponentController::fetchWeatherForLocation()
{
    Logger::LogInfo("Fetching weather for location...");
    m_isLoading = true;
    emit loadingStateChanged();

    m_locationService->fetchLocation();
}

void SetWeatherComponentController::onWeatherFetched(WeatherCondition condition, TemperatureCategory temperature)
{
    Logger::LogInfo("Weather fetched successfully.");
    m_weatherCondition = condition;
    m_temperatureCategory = temperature;
    m_isLoading = false;
    emit weatherDataChanged();
    emit loadingStateChanged();
}

void SetWeatherComponentController::saveWeatherData()
{
    WeatherData data(m_weatherCondition, m_temperatureCategory, QDateTime::currentDateTimeUtc().toString(Qt::ISODate));
    if (m_weatherRepository->saveWeatherData(data)) {
        Logger::LogInfo("Weather data saved successfully.");
    } else {
        Logger::LogWarn("Failed to save weather data.");
    }
}
