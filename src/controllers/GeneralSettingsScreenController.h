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
    Q_PROPERTY(int selectedCocktail1Id READ getSelectedCocktail1Id NOTIFY settingsChanged)
    Q_PROPERTY(int selectedCocktail2Id READ getSelectedCocktail2Id NOTIFY settingsChanged)
    Q_PROPERTY(int selectedCocktail3Id READ getSelectedCocktail3Id NOTIFY settingsChanged)
    Q_PROPERTY(int selectedCocktail4Id READ getSelectedCocktail4Id NOTIFY settingsChanged)
    Q_PROPERTY(int selectedCocktail5Id READ getSelectedCocktail5Id NOTIFY settingsChanged)
    Q_PROPERTY(int selectedCocktail6Id READ getSelectedCocktail6Id NOTIFY settingsChanged)

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

    int getSelectedCocktail1Id() const;
    int getSelectedCocktail2Id() const;
    int getSelectedCocktail3Id() const;
    int getSelectedCocktail4Id() const;
    int getSelectedCocktail5Id() const;
    int getSelectedCocktail6Id() const;

    Q_INVOKABLE void savePricePerCocktail(double price);
    Q_INVOKABLE void saveCupPawn(double pawn);
    Q_INVOKABLE void setSelectedCocktail(int index, int cocktailId);

signals:
    void settingsChanged();

private:
    QSharedPointer<SettingsRepository> m_settingsRepository;
    QSharedPointer<CocktailRepository> m_cocktailRepository;

    QSharedPointer<GeneralSettings> m_generalSettings;

    void loadSettings();
};
