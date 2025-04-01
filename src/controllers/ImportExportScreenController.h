#pragma once

#include "CocktailRepository.h"
#include "DiscountsRepository.h"
#include "SettingsRepository.h"

#include <QObject>
#include <QSharedPointer>
#include <QString>

class ImportExportScreenController : public QObject
{
    Q_OBJECT
public:
    explicit ImportExportScreenController(
        QSharedPointer<SettingsRepository> settingsRepository,
        QSharedPointer<CocktailRepository> cocktailRepository,
        QSharedPointer<DiscountsRepository> discountsRepository,
        QObject *parent = nullptr);

    Q_INVOKABLE void exportData(const QString &filePath);
    Q_INVOKABLE void importData(const QString &filePath);

signals:
    void operationStatus(const QString &message);

private:
    QString generateMD5Hash(const QString &filePath) const;
    bool verifyMD5Hash(const QString &data, const QString &expectedHash) const;
    void bundleData(const QString &exportFilePath, const QString &bundleFilePath) const;
    bool unbundleData(const QString &bundleFilePath, QString &exportedData, QString &hash) const;

    QSharedPointer<SettingsRepository> m_settingsRepository;
    QSharedPointer<CocktailRepository> m_cocktailRepository;
    QSharedPointer<DiscountsRepository> m_discountsRepository;
};
