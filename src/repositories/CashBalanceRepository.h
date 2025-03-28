#pragma once

#include "../DatabaseManager.h"

#include <QObject>
#include <QSharedPointer>

class CashBalanceRepository : public QObject
{
    Q_OBJECT
public:
    explicit CashBalanceRepository(QSharedPointer<DatabaseManager> dbManager, QObject *parent = nullptr);

    double getBalance() const;
    bool updateBalance(double newBalance, const QString &reason);
    bool adjustBalance(double amount, const QString &reason);

signals:
    void cashBalanceUpdate();

private:
    QSharedPointer<DatabaseManager> m_databaseManager;
};
