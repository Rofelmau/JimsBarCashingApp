#include "WeatherData.h"

WeatherData::WeatherData(WeatherCondition condition, TemperatureCategory temperature, const QString &timestamp)
    : condition(condition)
    , temperature(temperature)
    , timestamp(timestamp)
{
}

WeatherCondition WeatherData::getCondition() const {
    return condition;
}

TemperatureCategory WeatherData::getTemperature() const {
    return temperature;
}

QString WeatherData::getTimestamp() const {
    return timestamp;
}
