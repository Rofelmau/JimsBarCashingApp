#include "CocktailModel.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

CocktailModel::CocktailModel(const QSet<QSharedPointer<Cocktail>>& cocktails, QObject *parent)
    : QAbstractListModel(parent)
{
    m_cocktails = QVector<QSharedPointer<Cocktail>>::fromList(cocktails.values());
}

CocktailModel::~CocktailModel()
{
}

int CocktailModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_cocktails.size();
}

QVariant CocktailModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    QSharedPointer<Cocktail> cocktail = m_cocktails[index.row()];

    switch (role) {
    case NameRole:
        return cocktail->getName();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> CocktailModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    return roles;
}