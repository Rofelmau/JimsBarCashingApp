#include "DiscountType.h"

#include <array>
#include <stdexcept>

namespace
{
    constexpr std::array<std::pair<DiscountTypeHelper::DiscountType, const char *>, 4> DiscountTypeStrings = {
        {
            {DiscountTypeHelper::DiscountType::ClassicDiscount, "Klassischer Rabatt"},
            {DiscountTypeHelper::DiscountType::GroupDiscount, "Gruppen Rabatt"},
            {DiscountTypeHelper::DiscountType::ForFree, "Gratis"},
            {DiscountTypeHelper::DiscountType::PercentageDiscount, "Prozentualer Rabatt"},
        }
    };

}

bool DiscountTypeHelper::isValidDiscountType(int value)
{
    const QMetaEnum metaEnum = QMetaEnum::fromType<DiscountType>();
    return metaEnum.isValid() && metaEnum.valueToKey(value) != nullptr;
}

QString DiscountTypeHelper::toString(DiscountType type)
{
    for (const auto &pair : DiscountTypeStrings)
    {
        if (pair.first == type)
        {
            return QString(pair.second);
        }
    }
    throw std::invalid_argument("Invalid DiscountType enum value");
}

DiscountTypeHelper::DiscountType DiscountTypeHelper::fromString(const QString &type)
{
    for (const auto &pair : DiscountTypeStrings)
    {
        if (type == pair.second)
        {
            return pair.first;
        }
    }
    throw std::invalid_argument("Invalid DiscountType string " + type.toStdString());
}

QStringList DiscountTypeHelper::values()
{
    QStringList list;
    for (const auto &pair : DiscountTypeStrings)
    {
        list.append(pair.second);
    }
    return list;
}
