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
        PerPiece = 0,
        FixedPrice,
        Percentage
    };
    Q_ENUM(DiscountType)

    explicit DiscountTypeHelper(QObject *parent = nullptr) : QObject(parent) {}

    Q_INVOKABLE static QStringList values();
    Q_INVOKABLE static QString toString(DiscountType type);

    static bool isValidDiscountType(int value);
    static DiscountType fromString(const QString &type);
};

using DiscountType = DiscountTypeHelper::DiscountType;
