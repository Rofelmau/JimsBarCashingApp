#pragma once

#include <Cocktail.h>

#include <QAbstractListModel>
#include <QObject>
#include <QString>
#include <QSet>
#include <QSharedPointer>
#include <QVector>


class CocktailModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum CocktailRoles {
        NameRole = Qt::UserRole + 1
    };

    explicit CocktailModel(const QSet<QSharedPointer<Cocktail>>& cocktails, QObject *parent = nullptr);
    ~CocktailModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    QVector<QSharedPointer<Cocktail>> m_cocktails;
};