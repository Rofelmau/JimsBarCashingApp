#pragma once

#include "Discount.h"

#include <QAbstractListModel>
#include <QSharedPointer>

class DiscountModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum DiscountRoles
    {
        IdRole = Qt::UserRole + 1,
        NameRole,
        TypeRole,
        TypeValueRole,
        ValueRole,
        CocktailLimitRole
    };

    explicit DiscountModel(QObject *parent = nullptr);

    void setDiscounts(const QList<QSharedPointer<Discount>> &discounts);
    void updateDiscount(const QSharedPointer<Discount> &discount, int row);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<QSharedPointer<Discount>> m_discounts;
};
