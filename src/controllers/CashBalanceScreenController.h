#pragma once

#include "../repositories/CashBalanceRepository.h"

#include <QObject>
#include <QSharedPointer>

class CashBalanceScreenController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double balance READ balance NOTIFY balanceChanged)

public:
    explicit CashBalanceScreenController(QSharedPointer<CashBalanceRepository> repository, QObject *parent = nullptr);

    double balance() const;
    Q_INVOKABLE void modifyBalance(double amount, const QString &reason);

signals:
    void balanceChanged();

private:
    QSharedPointer<CashBalanceRepository> m_cashBalanceRepository;
    double m_balance;
    void fetchBalance();
};