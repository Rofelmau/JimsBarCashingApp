#include "CashBalanceRepository.h"

#include <QSqlQuery>
#include <QVariant>

CashBalanceRepository::CashBalanceRepository(QSharedPointer<DatabaseManager> dbManager, QObject *parent)
    : QObject(parent)
    , m_databaseManager(dbManager)
{
}

double CashBalanceRepository::getBalance() const
{
    QSqlQuery query(m_databaseManager->database());
    query.prepare("SELECT balance FROM CashRegister ORDER BY id DESC LIMIT 1");
    if (query.exec() && query.next()) {
        return query.value(0).toDouble();
    }
    return 0.0;
}

bool CashBalanceRepository::updateBalance(double newBalance, const QString &reason)
{
    QSqlQuery query(m_databaseManager->database());
    query.prepare("INSERT INTO CashRegister (balance) VALUES (:balance)");
    query.bindValue(":balance", newBalance);
    if (!query.exec()) {
        return false;
    }

    query.prepare("INSERT INTO CashRegisterLog (change_amount, new_balance, reason) VALUES (:change_amount, :new_balance, :reason)");
    query.bindValue(":change_amount", newBalance - getBalance());
    query.bindValue(":new_balance", newBalance);
    query.bindValue(":reason", reason);
    
    if (query.exec()) {
        emit cashBalanceUpdate();
        return true;
    }
    return false;
}

bool CashBalanceRepository::adjustBalance(double amount, const QString &reason)
{
    double currentBalance = getBalance();
    double newBalance = currentBalance + amount;

    QSqlQuery query(m_databaseManager->database());
    query.prepare("INSERT INTO CashRegister (balance) VALUES (:balance)");
    query.bindValue(":balance", newBalance);
    if (!query.exec()) {
        return false;
    }

    query.prepare("INSERT INTO CashRegisterLog (change_amount, new_balance, reason) VALUES (:change_amount, :new_balance, :reason)");
    query.bindValue(":change_amount", amount);
    query.bindValue(":new_balance", newBalance);
    query.bindValue(":reason", reason);
    
    if (query.exec()) {
        emit cashBalanceUpdate();
        return true;
    }
    return false;
}