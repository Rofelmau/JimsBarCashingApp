#pragma once

#include "../DatabaseManager.h"
#include "../entities/GeneralSettings.h"
#include "../repositories/CocktailRepository.h"

#include <QObject>
#include <QSharedPointer>

class SettingsRepository : public QObject
{
    Q_OBJECT
public:
    explicit SettingsRepository(QSharedPointer<DatabaseManager> dbManager
                                , QSharedPointer<CocktailRepository> cocktailRepository
                                , QObject *parent = nullptr);

    QSharedPointer<GeneralSettings> getGeneralSettings() const;

    void updatePricePerCocktail(double price);
    void updateCupPawn(double pawn);
    void updateSelectedCocktail(int index, QSharedPointer<Cocktail> cocktail);

private:
    QSharedPointer<DatabaseManager> dbManager;
    QSharedPointer<CocktailRepository> m_cocktailRepository;
};
