#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDir>
#include "cocktailmodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    QString filePath = QDir(QCoreApplication::applicationDirPath()).filePath("cocktails.json");
    CocktailModel cocktailModel(filePath);

    engine.rootContext()->setContextProperty("cocktailModel", &cocktailModel);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
