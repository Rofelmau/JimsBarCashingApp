#include "WeatherCondition.h"


WeatherConditionHelper::WeatherConditionHelper(QObject *parent)
    : QObject(parent)
{
}    

QString WeatherConditionHelper::weatherConditionToString(WeatherCondition condition)
{
    switch (condition) {
        case WeatherConditionHelper::WeatherCondition::Sunny: return "Sonnig ☀️";
        case WeatherConditionHelper::WeatherCondition::Cloudy: return "Bewölkt ☁️";
        case WeatherConditionHelper::WeatherCondition::Rainy: return "Regnerisch 🌧";
        case WeatherConditionHelper::WeatherCondition::Stormy: return "Unwetter ⛈️";
        case WeatherConditionHelper::WeatherCondition::Snowy: return "Schnee 🌨";
        default: return "Unbekannt";
    }
}
