#pragma once

#include <QMetaEnum>
#include <QObject>
#include <QString>
#include <QList>
#include <QVariant>

class WeatherConditionHelper : public QObject
{
    Q_OBJECT
public:
    enum class WeatherCondition {
        Sunny = 0,
        Cloudy = 1,
        Rainy = 2,
        Stormy = 3,
        Snowy = 4
    };
    Q_ENUM(WeatherCondition)

    explicit WeatherConditionHelper(QObject *parent = nullptr);

    Q_INVOKABLE static QString weatherConditionToString(WeatherCondition condition);

    Q_INVOKABLE static QVariantList values();
};

using WeatherCondition = WeatherConditionHelper::WeatherCondition;