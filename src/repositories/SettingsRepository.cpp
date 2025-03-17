#include "SettingsRepository.h"

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