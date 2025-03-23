#include "DatabaseManager.h"

#include "Logger.h"

#include <QSqlDatabase>
#include <QSqlError>

DatabaseManager::DatabaseManager()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("cocktails.db");

    if (!db.open()) {
        Logger::LogError("connection with database failed");
    } else {
        Logger::LogInfo("Database: connection ok");
    }
}

DatabaseManager::~DatabaseManager()
{
    db.close();
}

bool DatabaseManager::isOpen() const
{
    return db.isOpen();
}

QSqlDatabase DatabaseManager::database() const
{
    return db;
}
