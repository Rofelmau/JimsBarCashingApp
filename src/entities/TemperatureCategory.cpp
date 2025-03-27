#include "TemperatureCategory.h"

TemperatureCategoryHelper::TemperatureCategoryHelper(QObject *parent)
    : QObject(parent)
{
}    

QString TemperatureCategoryHelper::temperatureCategoryToString(TemperatureCategory category)
{
    switch (category) {
        case TemperatureCategoryHelper::TemperatureCategory::VeryCold: return "Sehr kalt \U0001F976"; // 🥶
        case TemperatureCategoryHelper::TemperatureCategory::Cool: return "Kühl ❄️";
        case TemperatureCategoryHelper::TemperatureCategory::Pleasant: return "Angenehm \U0001F324"; // 🌤
        case TemperatureCategoryHelper::TemperatureCategory::Warm: return "Warm \U0001F525"; // 🔥
        case TemperatureCategoryHelper::TemperatureCategory::Hot: return "Heiß \U0001F975"; // 🥵
        default: return "Unbekannt";
    }
}

QVariantList TemperatureCategoryHelper::values()
{
    QVariantList values;
    QMetaEnum metaEnum = QMetaEnum::fromType<TemperatureCategoryHelper::TemperatureCategory>();

    for (int i = 0; i < metaEnum.keyCount(); ++i) {
        values.append(QVariant::fromValue(static_cast<TemperatureCategoryHelper::TemperatureCategory>(metaEnum.value(i))));
    }

    return values;
}
