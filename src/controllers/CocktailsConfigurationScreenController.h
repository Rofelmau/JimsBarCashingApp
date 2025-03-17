#pragma once

#include <CocktailRepository.h>

#include <QObject>

class CocktailsConfigurationScreenController : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariantList cocktails READ cocktails NOTIFY cocktailsChanged)

public:
    explicit CocktailsConfigurationScreenController(QSharedPointer<CocktailRepository> cocktailRepository, QObject *parent = nullptr);

    Q_INVOKABLE void addNewCocktail(const QString &name, const QStringList &ingredients);
    Q_INVOKABLE void editCocktail(int id, const QString &name, const QStringList &ingredients);
    Q_INVOKABLE void deleteCocktail(int id);

signals:
    void cocktailsChanged();

private:
    QVariantList cocktails() const;

    QSharedPointer<CocktailRepository> m_cocktailRepository;
};
