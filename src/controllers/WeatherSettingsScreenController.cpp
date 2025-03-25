#include "WeatherSettingsScreenController.h"

#include "../entities/TemperatureCategory.h"
#include "../entities/WeatherCondition.h"

#include <QDateTime>

WeatherSettingsScreenController::WeatherSettingsScreenController(QSharedPointer<WeatherRepository> weatherRepository, QObject *parent)
    : QObject(parent)
    , m_weatherRepository(weatherRepository)
{
    QObject::connect(m_weatherRepository.data(), &WeatherRepository::weatherDataUpdated,
                     this, &WeatherSettingsScreenController::fetchLatestWeather);
}

bool WeatherSettingsScreenController::hasWeatherData() const
{
    return m_hasWeatherData;
}

QString WeatherSettingsScreenController::lastWeatherCondition() const
{
    return m_lastWeatherCondition;
}

QString WeatherSettingsScreenController::lastTemperature() const
{
    return m_lastTemperature;
}

void WeatherSettingsScreenController::fetchLatestWeather()
{
    auto weatherDataList = m_weatherRepository->fetchWeatherData();

    if (!weatherDataList.isEmpty()) {
        const auto &latestWeather = weatherDataList.first();

        // Check if the weather data is from today
        QDateTime weatherTimestamp = QDateTime::fromString(latestWeather.getTimestamp(), Qt::ISODate);

        if (weatherTimestamp.date() == QDate::currentDate()) {
            m_hasWeatherData = true;

            m_lastWeatherCondition = WeatherConditionHelper::weatherConditionToString(latestWeather.getCondition());
            m_lastTemperature = TemperatureCategoryHelper::temperatureCategoryToString(latestWeather.getTemperature());
        } else {
            m_hasWeatherData = false;
            m_lastWeatherCondition.clear();
            m_lastTemperature.clear();
        }
    } else {
        m_hasWeatherData = false;
        m_lastWeatherCondition.clear();
        m_lastTemperature.clear();
    }

    emit weatherDataChanged();
}
