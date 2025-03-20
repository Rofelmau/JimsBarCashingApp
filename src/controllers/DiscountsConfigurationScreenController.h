#pragma once

#include "DiscountModel.h"
#include "DiscountsRepository.h"

#include <QObject>
#include <QSharedPointer>

class DiscountsConfigurationScreenController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(DiscountModel *discountModel READ discountModel NOTIFY discountsChanged)

public:
    explicit DiscountsConfigurationScreenController(QSharedPointer<DiscountsRepository> discountsRepository, QObject *parent = nullptr);

    DiscountModel *discountModel() const;

    Q_INVOKABLE void addDiscount(const QString &name, const QString &type, double value, int cocktailLimit, bool reusable, bool combinable);
    Q_INVOKABLE void updateDiscount(int id, const QString &name, const QString &type, double value, int cocktailLimit, bool reusable, bool combinable);
    Q_INVOKABLE void deleteDiscount(int id);

signals:
    void discountsChanged();

private:
    QSharedPointer<DiscountsRepository> m_discountsRepository;
    QScopedPointer<DiscountModel> m_discountModel;

    void loadDiscounts();
};
