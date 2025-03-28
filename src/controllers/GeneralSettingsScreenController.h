#pragma once

#include "../repositories/CocktailRepository.h"
#include "../repositories/SettingsRepository.h"

#include <QObject>
#include <QSharedPointer>

class GeneralSettingsScreenController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double pricePerCocktail READ getPricePerCocktail NOTIFY settingsChanged)
    Q_PROPERTY(double cupPawn READ getCupPawn NOTIFY settingsChanged)
    Q_PROPERTY(QString selectedCocktail1 READ getSelectedCocktail1 NOTIFY settingsChanged)
    Q_PROPERTY(QString selectedCocktail2 READ getSelectedCocktail2 NOTIFY settingsChanged)
    Q_PROPERTY(QString selectedCocktail3 READ getSelectedCocktail3 NOTIFY settingsChanged)
    Q_PROPERTY(QString selectedCocktail4 READ getSelectedCocktail4 NOTIFY settingsChanged)
    Q_PROPERTY(QString selectedCocktail5 READ getSelectedCocktail5 NOTIFY settingsChanged)
    Q_PROPERTY(QString selectedCocktail6 READ getSelectedCocktail6 NOTIFY settingsChanged)
    Q_PROPERTY(QVariantList availableCocktails READ getAvailableCocktails NOTIFY settingsChanged)
    Q_PROPERTY(QString selectedCocktail1Uuid READ getSelectedCocktail1Uuid NOTIFY settingsChanged)
    Q_PROPERTY(QString selectedCocktail2Uuid READ getSelectedCocktail2Uuid NOTIFY settingsChanged)
    Q_PROPERTY(QString selectedCocktail3Uuid READ getSelectedCocktail3Uuid NOTIFY settingsChanged)
    Q_PROPERTY(QString selectedCocktail4Uuid READ getSelectedCocktail4Uuid NOTIFY settingsChanged)
    Q_PROPERTY(QString selectedCocktail5Uuid READ getSelectedCocktail5Uuid NOTIFY settingsChanged)
    Q_PROPERTY(QString selectedCocktail6Uuid READ getSelectedCocktail6Uuid NOTIFY settingsChanged)

public:
    explicit GeneralSettingsScreenController(QSharedPointer<SettingsRepository> settingsRepo
                                            , QSharedPointer<CocktailRepository> cocktailRepo
                                            , QObject *parent = nullptr);

    double getPricePerCocktail() const;
    double getCupPawn() const;
    QString getSelectedCocktail1() const;
    QString getSelectedCocktail2() const;
    QString getSelectedCocktail3() const;
    QString getSelectedCocktail4() const;
    QString getSelectedCocktail5() const;
    QString getSelectedCocktail6() const;
    QVariantList getAvailableCocktails() const;

    QString getSelectedCocktail1Uuid() const;
    QString getSelectedCocktail2Uuid() const;
    QString getSelectedCocktail3Uuid() const;
    QString getSelectedCocktail4Uuid() const;
    QString getSelectedCocktail5Uuid() const;
    QString getSelectedCocktail6Uuid() const;

    Q_INVOKABLE void savePricePerCocktail(double price);
    Q_INVOKABLE void saveCupPawn(double pawn);
    Q_INVOKABLE void setSelectedCocktail(int index, const QString &cocktailUuid);

signals:
    void settingsChanged();

private:
    QSharedPointer<SettingsRepository> m_settingsRepository;
    QSharedPointer<CocktailRepository> m_cocktailRepository;

    QSharedPointer<GeneralSettings> m_generalSettings;

    void loadSettings();
};
