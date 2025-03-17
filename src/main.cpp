#include "DatabaseManager.h"

#include "CocktailRepository.h"
#include "SalesRepository.h"
#include "SettingsRepository.h"

#include "CheckoutScreenController.h"
#include "CocktailsConfigurationScreenController.h"
#include "GeneralSettingsScreenController.h"

#include <QApplication>
#include <QGuiApplication>
#include <QLibraryInfo>
#include <QLocale>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QQmlApplicationEngine engine;

    QLocale::setDefault(QLocale(QLocale::German, QLocale::Germany));
    QTranslator translator;
    if (translator.load(QLocale(), "qtbase", "_", QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
    {
        app.installTranslator(&translator);
    }

    QSharedPointer<DatabaseManager> dbManager{new DatabaseManager};

    QSharedPointer<CocktailRepository> cocktailRepository{new CocktailRepository{dbManager}};
    QSharedPointer<SalesRepository> salesRepository{new SalesRepository{dbManager}};
    QSharedPointer<SettingsRepository> settingsRepository{new SettingsRepository{dbManager, cocktailRepository}};

    CocktailsConfigurationScreenController cocktailsConfigurationScreenController(cocktailRepository);
    engine.rootContext()->setContextProperty("CocktailsConfigurationScreenController", &cocktailsConfigurationScreenController);
    
    GeneralSettingsScreenController generalSettingsScreenController{settingsRepository, cocktailRepository};
    engine.rootContext()->setContextProperty("GeneralSettingsScreenController", &generalSettingsScreenController);

    CheckoutScreenController checkoutScreenController{settingsRepository, cocktailRepository, salesRepository};
    engine.rootContext()->setContextProperty("CheckoutScreenController", &checkoutScreenController);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
