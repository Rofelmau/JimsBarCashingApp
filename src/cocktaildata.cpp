#include "cocktaildata.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

CocktailData::CocktailData(QObject *parent) : QObject(parent) {
    loadCocktails();
}

QVariantList CocktailData::cocktails() const {
    return m_cocktails;
}

void CocktailData::loadCocktails() {
    QFile file(":/cocktails.json");
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open cocktails.json file.");
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(data));
    QJsonArray array = doc.array();

    for (const QJsonValue &value : array) {
        QJsonObject obj = value.toObject();
        QVariantMap map;
        map["name"] = obj["name"].toString();
        map["ingredients"] = obj["ingredients"].toString();
        m_cocktails.append(map);
    }

    emit cocktailsChanged();
}