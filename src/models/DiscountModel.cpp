#include "DiscountModel.h"

#include "DiscountType.h"

#include <QAbstractListModel>
#include <QSharedPointer>

DiscountModel::DiscountModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

void DiscountModel::setDiscounts(const QList<QSharedPointer<Discount>> &discounts)
{
    beginResetModel();
    m_discounts = discounts;
    endResetModel();
}

void DiscountModel::updateDiscount(const QSharedPointer<Discount> &discount, int row)
{
    if (row < 0 || row >= m_discounts.size())
    {
        return;
    }

    m_discounts[row] = discount;
    emit dataChanged(index(row), index(row));
}

int DiscountModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_discounts.size();
}

QVariant DiscountModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_discounts.size()) {
        return QVariant();
    }

    QSharedPointer<Discount> discount = m_discounts.at(index.row());

    switch (role)
    {
    case IdRole:
        return discount->getUuid();
    case NameRole:
        return discount->getName();
    case TypeRole:
        return DiscountTypeHelper::toString(discount->getType());
    case TypeValueRole:
        return static_cast<int>(discount->getType());
    case ValueRole:
        return discount->getValue();
    case CocktailLimitRole:
        return discount->getCocktailLimit();
    case ActiveRole:
        return discount->isActive();
    }

    return QVariant();
}

QHash<int, QByteArray> DiscountModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "discountId";
    roles[NameRole] = "name";
    roles[TypeRole] = "type";
    roles[TypeValueRole] = "typeValue";
    roles[ValueRole] = "value";
    roles[CocktailLimitRole] = "cocktailLimit";
    roles[ActiveRole] = "active";
    return roles;
}
