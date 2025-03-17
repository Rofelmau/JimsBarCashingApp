#include "DatabaseManager.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QCoreApplication>
#include <QDebug>

DatabaseManager::DatabaseManager()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("cocktails.db");

    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    } else {
        qDebug() << "Database: connection ok";
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
