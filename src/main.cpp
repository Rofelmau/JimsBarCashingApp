#include "Logger.h"

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
    LOG_FUNCTION();

    Logger::LogInfo("Application starting...");

    QApplication app(argc, argv);
    QQmlApplicationEngine engine;

    QLocale::setDefault(QLocale(QLocale::German, QLocale::Germany));
    QTranslator translator;
    if (translator.load(QLocale(), "qtbase", "_", QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
    {
        app.installTranslator(&translator);
        Logger::LogInfo("Translator loaded successfully.");
    }
    else
    {
        Logger::LogWarn("Failed to load translator.");
    }

    Logger::LogInfo("Initializing database manager...");
    QSharedPointer<DatabaseManager> dbManager{new DatabaseManager};

    Logger::LogInfo("Setting up repositories...");
    QSharedPointer<CocktailRepository> cocktailRepository{new CocktailRepository{dbManager}};
    QSharedPointer<SalesRepository> salesRepository{new SalesRepository{dbManager}};
    QSharedPointer<SettingsRepository> settingsRepository{new SettingsRepository{dbManager, cocktailRepository}};

    Logger::LogInfo("Setting up controllers...");
    CocktailsConfigurationScreenController cocktailsConfigurationScreenController(cocktailRepository);
    engine.rootContext()->setContextProperty("CocktailsConfigurationScreenController", &cocktailsConfigurationScreenController);
    
    GeneralSettingsScreenController generalSettingsScreenController{settingsRepository, cocktailRepository};
    engine.rootContext()->setContextProperty("GeneralSettingsScreenController", &generalSettingsScreenController);

    CheckoutScreenController checkoutScreenController{settingsRepository, cocktailRepository, salesRepository};
    engine.rootContext()->setContextProperty("CheckoutScreenController", &checkoutScreenController);

    Logger::LogInfo("Loading QML file...");
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty()) {
        Logger::LogError("Failed to load QML file. Exiting application.");
        return -1;
    }

    Logger::LogInfo("Application started successfully.");
    return app.exec();
}
