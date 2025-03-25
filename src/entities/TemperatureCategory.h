#pragma once

#include <QMetaEnum>
#include <QObject>
#include <QString>

class TemperatureCategoryHelper : public QObject
{
    Q_OBJECT
public:
    enum class TemperatureCategory {
        VeryCold = 0,
        Cool = 1,
        Pleasant = 2,
        Warm = 3,
        Hot = 4
    };
    Q_ENUM(TemperatureCategory)

    explicit TemperatureCategoryHelper(QObject *parent = nullptr);

    Q_INVOKABLE static QString temperatureCategoryToString(TemperatureCategory category);
};

using TemperatureCategory = TemperatureCategoryHelper::TemperatureCategory;
