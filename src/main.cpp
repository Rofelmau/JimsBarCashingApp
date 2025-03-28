#include "Logger.h"

#include "DatabaseManager.h"

#include "CashBalanceRepository.h"
#include "CocktailRepository.h"
#include "DiscountsRepository.h"
#include "SalesRepository.h"
#include "SettingsRepository.h"
#include "WeatherRepository.h"

#include "CashBalanceScreenController.h"
#include "CheckoutScreenController.h"
#include "CocktailsConfigurationScreenController.h"
#include "DiscountsConfigurationScreenController.h"
#include "GeneralSettingsScreenController.h"
#include "ImportExportScreenController.h"
#include "SetWeatherComponentController.h"
#include "StatisticsScreenController.h"
#include "WeatherSettingsScreenController.h"

#include "LocationService.h"
#include "WeatherService.h"

#include "DiscountModel.h"
#include "entities/DiscountType.h"
#include "entities/TemperatureCategory.h"
#include "entities/WeatherCondition.h"

#include <QApplication>
#include <QGuiApplication>
#include <QLibraryInfo>
#include <QLocale>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTranslator>
#include <QSslSocket>

int main(int argc, char *argv[])
{
    LOG_FUNCTION();

    if (!QSslSocket::supportsSsl()) {
        Logger::LogError("SSL/TLS support is not available. Please install OpenSSL.");
        return -1;
    }

    Logger::LogInfo("SSL/TLS support is available.");

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

    Logger::LogInfo("Registering types...");
    qRegisterMetaType<WeatherCondition>("WeatherCondition");
    qRegisterMetaType<TemperatureCategory>("TemperatureCategory");

    Logger::LogInfo("Initializing database manager...");
    QSharedPointer<DatabaseManager> dbManager{new DatabaseManager};

    Logger::LogInfo("Setting up repositories...");
    QSharedPointer<CashBalanceRepository> cashBalanceRepository{new CashBalanceRepository{dbManager}};
    QSharedPointer<CocktailRepository> cocktailRepository{new CocktailRepository{dbManager}};
    QSharedPointer<DiscountsRepository> discountsRepository{new DiscountsRepository{dbManager}};
    QSharedPointer<SalesRepository> salesRepository{new SalesRepository{dbManager}};
    QSharedPointer<SettingsRepository> settingsRepository{new SettingsRepository{dbManager, cocktailRepository}};
    QSharedPointer<WeatherRepository> weatherRepository{new WeatherRepository{dbManager}};

    Logger::LogInfo("Setting up controllers...");
    CashBalanceScreenController cashBalanceScreenController{cashBalanceRepository};
    engine.rootContext()->setContextProperty("CashBalanceScreenController", &cashBalanceScreenController);

    CocktailsConfigurationScreenController cocktailsConfigurationScreenController{cocktailRepository};
    engine.rootContext()->setContextProperty("CocktailsConfigurationScreenController", &cocktailsConfigurationScreenController);

    GeneralSettingsScreenController generalSettingsScreenController{settingsRepository, cocktailRepository};
    engine.rootContext()->setContextProperty("GeneralSettingsScreenController", &generalSettingsScreenController);

    WeatherSettingsScreenController weatherSettingsScreenController{weatherRepository};
    engine.rootContext()->setContextProperty("WeatherSettingsScreenController", &weatherSettingsScreenController);

    CheckoutScreenController checkoutScreenController{settingsRepository, cocktailRepository, salesRepository, discountsRepository, cashBalanceRepository};
    engine.rootContext()->setContextProperty("CheckoutScreenController", &checkoutScreenController);

    StatisticsScreenController statisticsScreenController{salesRepository, weatherRepository};
    engine.rootContext()->setContextProperty("StatisticsScreenController", &statisticsScreenController);

    DiscountsConfigurationScreenController discountsConfigurationScreenController{discountsRepository};
    engine.rootContext()->setContextProperty("DiscountsConfigurationScreenController", &discountsConfigurationScreenController);

    QSharedPointer<WeatherService> weatherService{new WeatherService};
    QSharedPointer<LocationService> locationService{new LocationService};
    SetWeatherComponentController setWeatherComponentController{weatherService, locationService, weatherRepository};
    engine.rootContext()->setContextProperty("SetWeatherComponentController", &setWeatherComponentController);

    ImportExportScreenController importExportScreenController{settingsRepository, cocktailRepository, discountsRepository};
    engine.rootContext()->setContextProperty("ImportExportScreenController", &importExportScreenController);

    Logger::LogInfo("Registering Models to QML ...");
    qmlRegisterType<DiscountModel>("JimsBarCashingApp", 1, 0, "DiscountModel");

    Logger::LogInfo("Registering Helpers to QML ...");
    qmlRegisterType<DiscountTypeHelper>("App.Helpers", 1, 0, "DiscountTypeHelper");
    qmlRegisterType<TemperatureCategoryHelper>("App.Helpers", 1, 0, "TemperatureCategoryHelper");
    qmlRegisterType<WeatherConditionHelper>("App.Helpers", 1, 0, "WeatherConditionHelper");

    Logger::LogInfo("Loading QML file...");
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
    {
        Logger::LogError("Failed to load QML file. Exiting application.");
        return -1;
    }

    Logger::LogInfo("Application started successfully.");
    return app.exec();
}
