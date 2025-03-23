#pragma once

#include <QMetaEnum>
#include <QObject>
#include <QString>
#include <QStringList>

class DiscountTypeHelper : public QObject
{
    Q_OBJECT
public:
    enum class DiscountType
    {
        ClassicDiscount = 0,
        GroupDiscount,
        ForFree,
        PercentageDiscount
    };
    Q_ENUM(DiscountType)

    explicit DiscountTypeHelper(QObject *parent = nullptr) : QObject(parent) {}

    Q_INVOKABLE static QStringList values();
    Q_INVOKABLE static QString toString(DiscountType type);
    Q_INVOKABLE static DiscountType fromString(const QString &type);


    static bool isValidDiscountType(int value);
};

using DiscountType = DiscountTypeHelper::DiscountType;
