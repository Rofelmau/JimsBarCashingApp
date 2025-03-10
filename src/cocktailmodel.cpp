#include "cocktailmodel.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

CocktailModel::CocktailModel(const QString &filePath, QObject *parent)
    : QAbstractListModel(parent) {
    loadCocktails(filePath);
}

void CocktailModel::loadCocktails(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open file.");
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(data));
    QJsonArray array = doc.array();

    for (const QJsonValue &value : array) {
        QJsonObject obj = value.toObject();
        Cocktail cocktail;
        cocktail.name = obj["name"].toString();
        cocktail.color = obj["color"].toString();
        cocktail.image = obj["image"].toString();
        m_cocktails.append(cocktail);
    }
}

int CocktailModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_cocktails.count();
}

QVariant CocktailModel::data(const QModelIndex &index, int role) const {
    if (index.row() < 0 || index.row() >= m_cocktails.count())
        return QVariant();

    const Cocktail &cocktail = m_cocktails[index.row()];
    if (role == NameRole)
        return cocktail.name;
    else if (role == ColorRole)
        return cocktail.color;
    else if (role == ImageRole)
        return cocktail.image;
    return QVariant();
}

QHash<int, QByteArray> CocktailModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[ColorRole] = "color";
    roles[ImageRole] = "image";
    return roles;
}