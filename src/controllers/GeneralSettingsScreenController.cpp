#include "GeneralSettingsScreenController.h"

#include "../entities/Cocktail.h"
#include "../entities/GeneralSettings.h"

#include <QSet>
#include <QSharedPointer>

GeneralSettingsScreenController::GeneralSettingsScreenController(QSharedPointer<SettingsRepository> settingsRepo
                                                                , QSharedPointer<CocktailRepository> cocktailRepo
                                                                , QObject *parent)
    : QObject(parent)
    , m_settingsRepository(settingsRepo)
    , m_cocktailRepository(cocktailRepo)
{
    loadSettings();
}

void GeneralSettingsScreenController::loadSettings()
{
    m_generalSettings = m_settingsRepository->getGeneralSettings();
    emit settingsChanged();
}

double GeneralSettingsScreenController::getPricePerCocktail() const
{
    return m_generalSettings ? m_generalSettings->getPricePerCocktail() : 0.0;
}

void GeneralSettingsScreenController::savePricePerCocktail(const double price)
{
    m_settingsRepository->updatePricePerCocktail(price);
    loadSettings();
}

double GeneralSettingsScreenController::getCupPawn() const
{
    return m_generalSettings ? m_generalSettings->getCupPawn() : 0.0;
}

void GeneralSettingsScreenController::saveCupPawn(const double pawn)
{
    m_settingsRepository->updateCupPawn(pawn);
    loadSettings();
}

QString GeneralSettingsScreenController::getSelectedCocktail1() const
{
    auto cocktail = m_generalSettings ? m_generalSettings->getSelectedCocktail(1) : nullptr;
    return cocktail ? cocktail->getName() : "None";
}

QString GeneralSettingsScreenController::getSelectedCocktail2() const
{
    auto cocktail = m_generalSettings ? m_generalSettings->getSelectedCocktail(2) : nullptr;
    return cocktail ? cocktail->getName() : "None";
}

QString GeneralSettingsScreenController::getSelectedCocktail3() const
{
    auto cocktail = m_generalSettings ? m_generalSettings->getSelectedCocktail(3) : nullptr;
    return cocktail ? cocktail->getName() : "None";
}

QString GeneralSettingsScreenController::getSelectedCocktail4() const
{
    auto cocktail = m_generalSettings ? m_generalSettings->getSelectedCocktail(4) : nullptr;
    return cocktail ? cocktail->getName() : "None";
}

QString GeneralSettingsScreenController::getSelectedCocktail5() const
{
    auto cocktail = m_generalSettings ? m_generalSettings->getSelectedCocktail(5) : nullptr;
    return cocktail ? cocktail->getName() : "None";
}

QString GeneralSettingsScreenController::getSelectedCocktail6() const
{
    auto cocktail = m_generalSettings ? m_generalSettings->getSelectedCocktail(6) : nullptr;
    return cocktail ? cocktail->getName() : "None";
}

QString GeneralSettingsScreenController::getSelectedCocktail1Uuid() const
{
    auto cocktail = m_generalSettings ? m_generalSettings->getSelectedCocktail(1) : nullptr;
    return cocktail ? cocktail->getUuid() : "";
}

QString GeneralSettingsScreenController::getSelectedCocktail2Uuid() const
{
    auto cocktail = m_generalSettings ? m_generalSettings->getSelectedCocktail(2) : nullptr;
    return cocktail ? cocktail->getUuid() : "";
}

QString GeneralSettingsScreenController::getSelectedCocktail3Uuid() const
{
    auto cocktail = m_generalSettings ? m_generalSettings->getSelectedCocktail(3) : nullptr;
    return cocktail ? cocktail->getUuid() : "";
}

QString GeneralSettingsScreenController::getSelectedCocktail4Uuid() const
{
    auto cocktail = m_generalSettings ? m_generalSettings->getSelectedCocktail(4) : nullptr;
    return cocktail ? cocktail->getUuid() : "";
}

QString GeneralSettingsScreenController::getSelectedCocktail5Uuid() const
{
    auto cocktail = m_generalSettings ? m_generalSettings->getSelectedCocktail(5) : nullptr;
    return cocktail ? cocktail->getUuid() : "";
}

QString GeneralSettingsScreenController::getSelectedCocktail6Uuid() const
{
    auto cocktail = m_generalSettings ? m_generalSettings->getSelectedCocktail(6) : nullptr;
    return cocktail ? cocktail->getUuid() : "";
}

QVariantList GeneralSettingsScreenController::getAvailableCocktails() const
{
    QVariantList cocktails;
    for (const QSharedPointer<Cocktail>& cocktail : m_cocktailRepository->getAllCocktails())
    {
        QVariantMap cocktailData;
        cocktailData["uuid"] = cocktail->getUuid();
        cocktailData["name"] = cocktail->getName();
        cocktails.append(cocktailData);
    }
    return cocktails;
}

void GeneralSettingsScreenController::setSelectedCocktail(int index, const QString &cocktailUuid)
{
    QSharedPointer<Cocktail> cocktail = cocktailUuid.isEmpty() ? nullptr : m_cocktailRepository->getCocktailByUuid(cocktailUuid);
    m_settingsRepository->updateSelectedCocktail(index, cocktail);
    loadSettings();
}
