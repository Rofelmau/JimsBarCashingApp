#pragma once

#include "../DatabaseManager.h"
#include "../entities/Sale.h"

#include <QObject>
#include <QSharedPointer>

class SalesRepository : public QObject
{
    Q_OBJECT
public:
    explicit SalesRepository(QSharedPointer<DatabaseManager> dbManager, QObject *parent = nullptr);

    void saveSale(const Sale &sale);

private:
    QSharedPointer<DatabaseManager> m_databaseManager;
};
