#pragma once

#include "WeatherCondition.h"
#include "TemperatureCategory.h"
#include <QString>

class WeatherData {
public:
    WeatherData(WeatherCondition condition, TemperatureCategory temperature, const QString &timestamp);

    WeatherCondition getCondition() const;
    TemperatureCategory getTemperature() const;
    QString getTimestamp() const;

private:
    WeatherCondition condition;
    TemperatureCategory temperature;
    QString timestamp;
};
