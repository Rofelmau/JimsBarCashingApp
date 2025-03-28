#include "SettingsRepository.h"

#include "../Logger.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

SettingsRepository::SettingsRepository(QSharedPointer<DatabaseManager> dbManager
                                        , QSharedPointer<CocktailRepository> cocktailRepository
                                        , QObject *parent)
    : QObject(parent)
    , dbManager(dbManager)
    , m_cocktailRepository(cocktailRepository)
{
}

QSharedPointer<GeneralSettings> SettingsRepository::getGeneralSettings() const
{
    QSharedPointer<GeneralSettings> settings = QSharedPointer<GeneralSettings>::create();

    QSqlQuery query(dbManager->database());
    query.prepare("SELECT key, value FROM Settings");

    if (query.exec())
    {
        while (query.next())
        {
            QString key = query.value("key").toString();
            QString value = query.value("value").toString();

            if (key == "price_per_cocktail")
            {
                settings->setPricePerCocktail(value.toDouble());
            }
            else if (key == "cup_pawn")
            {
                settings->setCupPawn(value.toDouble());
            }
            else if (key.startsWith("selected_cocktail_"))
            {
                bool ok;
                int index = key.mid(18).toInt(&ok);
                if (ok)
                {
                    int cocktailId = value.toInt();
                    QSharedPointer<Cocktail> cocktail = (cocktailId == -1) ? nullptr : m_cocktailRepository->getCocktailById(cocktailId);
                    settings->setSelectedCocktail(index, cocktail);
                }
            }
        }
    }

    return settings;
}

void SettingsRepository::updatePricePerCocktail(double price)
{
    QSqlQuery query(dbManager->database());
    query.prepare("UPDATE Settings SET value = :value WHERE key = 'price_per_cocktail'");
    query.bindValue(":value", QString::number(price, 'f', 2));
    query.exec();
}

void SettingsRepository::updateCupPawn(double pawn)
{
    QSqlQuery query(dbManager->database());
    query.prepare("UPDATE Settings SET value = :value WHERE key = 'cup_pawn'");
    query.bindValue(":value", QString::number(pawn, 'f', 2));
    query.exec();
}

void SettingsRepository::updateSelectedCocktail(const int index, QSharedPointer<Cocktail> cocktail)
{
    QSqlQuery query(dbManager->database());
    query.prepare("UPDATE Settings SET value = :value WHERE key = :key");
    query.bindValue(":key", QString("selected_cocktail_%1").arg(index));
    query.bindValue(":value", cocktail ? QString::number(cocktail->getId()) : "-1");
    query.exec();
}

QJsonObject SettingsRepository::exportAsJson() const
{
    QJsonObject generalSettings;
    QSharedPointer<GeneralSettings> settings = getGeneralSettings();

    generalSettings["price_per_cocktail"] = settings->getPricePerCocktail();
    generalSettings["cup_pawn"] = settings->getCupPawn();

    QJsonArray selectedCocktails;
    for (int i = 0; i < settings->getSelectedCocktails().size(); ++i) {
        QSharedPointer<Cocktail> cocktail = settings->getSelectedCocktail(i);
        selectedCocktails.append(cocktail ? cocktail->getId() : -1);
    }
    generalSettings["selected_cocktails"] = selectedCocktails;

    return generalSettings;
}

void SettingsRepository::import(const QJsonObject &json)
{
    if (json.contains("price_per_cocktail")) {
        updatePricePerCocktail(json["price_per_cocktail"].toDouble());
    }

    if (json.contains("cup_pawn")) {
        updateCupPawn(json["cup_pawn"].toDouble());
    }

    if (json.contains("selected_cocktails") && json["selected_cocktails"].isArray()) {
        QJsonArray selectedCocktails = json["selected_cocktails"].toArray();
        for (int i = 0; i < selectedCocktails.size(); ++i) {
            int cocktailId = selectedCocktails[i].toInt();
            QSharedPointer<Cocktail> cocktail = (cocktailId == -1) ? nullptr : m_cocktailRepository->getCocktailById(cocktailId);
            updateSelectedCocktail(i, cocktail);
        }
    }
}

int SettingsRepository::getDatabaseVersion() const
{
    QSqlQuery query(dbManager->database());
    query.prepare("SELECT version FROM DatabaseVersion ORDER BY id DESC LIMIT 1");

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }

    Logger::LogError("Failed to retrieve database version.");
    return -1;
}
