#pragma once

#include <QSqlDatabase>

class DatabaseManager
{
public:
    DatabaseManager();
    ~DatabaseManager();

    bool isOpen() const;
    QSqlDatabase database() const;

private:
    QSqlDatabase db;
};
