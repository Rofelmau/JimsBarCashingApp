#include "TemperatureCategory.h"

TemperatureCategoryHelper::TemperatureCategoryHelper(QObject *parent)
    : QObject(parent)
{
}    

QString TemperatureCategoryHelper::temperatureCategoryToString(TemperatureCategory category)
{
    switch (category) {
        case TemperatureCategoryHelper::TemperatureCategory::VeryCold: return "Sehr kalt 🥶";
        case TemperatureCategoryHelper::TemperatureCategory::Cool: return "Kühl ❄️";
        case TemperatureCategoryHelper::TemperatureCategory::Pleasant: return "Angenehm 🌤";
        case TemperatureCategoryHelper::TemperatureCategory::Warm: return "Warm 🔥";
        case TemperatureCategoryHelper::TemperatureCategory::Hot: return "Heiß 🥵";
        default: return "Unbekannt";
    }
}
