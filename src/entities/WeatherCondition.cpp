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
        case WeatherConditionHelper::WeatherCondition::Rainy: return "Regnerisch \U0001F327"; // 🌧
        case WeatherConditionHelper::WeatherCondition::Stormy: return "Unwetter \U0001F329"; // ⛈️
        case WeatherConditionHelper::WeatherCondition::Snowy: return "Schnee \U0001F328"; // 🌨
        default: return "Unbekannt";
    }
}

QVariantList WeatherConditionHelper::values()
{
    QVariantList values;
    QMetaEnum metaEnum = QMetaEnum::fromType<WeatherConditionHelper::WeatherCondition>();

    for (int i = 0; i < metaEnum.keyCount(); ++i) {
        values.append(QVariant::fromValue(static_cast<WeatherConditionHelper::WeatherCondition>(metaEnum.value(i))));
    }

    return values;
}
