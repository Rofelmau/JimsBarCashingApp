#include "CashBalanceScreenController.h"

CashBalanceScreenController::CashBalanceScreenController(QSharedPointer<CashBalanceRepository> repository, QObject *parent)
    : QObject(parent)
    , m_cashBalanceRepository(repository)
    , m_balance(0.0)
{
    fetchBalance();

    QObject::connect(m_cashBalanceRepository.data(), &CashBalanceRepository::cashBalanceUpdate,
                    this, &CashBalanceScreenController::fetchBalance);
}

double CashBalanceScreenController::balance() const
{
    return m_balance;
}

void CashBalanceScreenController::modifyBalance(double amount, const QString &reason)
{
    double newBalance = m_balance + amount;
    if (m_cashBalanceRepository->updateBalance(newBalance, reason)) {
        m_balance = newBalance;
        emit balanceChanged();
    }
}

void CashBalanceScreenController::fetchBalance()
{
    m_balance = m_cashBalanceRepository->getBalance();
    emit balanceChanged();
}