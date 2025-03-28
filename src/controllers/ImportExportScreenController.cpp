#include "ImportExportScreenController.h"

#include "../Logger.h"

#include <QCryptographicHash>
#include <QDateTime>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QTextStream>
#include <QUrl>

namespace {
    const QString EXPORT_TIMESTAMP_KEY = "export_timestamp";
    const QString DATABASE_VERSION_KEY = "database_version";
    const QString DATA_KEY = "data";
    const QString MD5_KEY = "md5";
    const QString COCKTAILS_KEY = "cocktails";
    const QString DISCOUNTS_KEY = "discounts";
    const QString GENERAL_SETTINGS_KEY = "generalSettings";
}

ImportExportScreenController::ImportExportScreenController(QSharedPointer<SettingsRepository> settingsRepository,
                                                            QSharedPointer<CocktailRepository> cocktailRepository,
                                                            QSharedPointer<DiscountsRepository> discountsRepository,
                                                            QObject *parent)
    : QObject(parent)
    , m_settingsRepository(settingsRepository)
    , m_cocktailRepository(cocktailRepository)
    , m_discountsRepository(discountsRepository)
{
}

QString ImportExportScreenController::generateMD5Hash(const QString &data) const
{
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(data.toUtf8());
    return hash.result().toHex();
}

bool ImportExportScreenController::verifyMD5Hash(const QString &data, const QString &expectedHash) const
{
    return generateMD5Hash(data) == expectedHash;
}

void ImportExportScreenController::bundleData(const QString &exportFilePath, const QString &bundleFilePath) const
{
    QFile exportFile(exportFilePath);
    if (!exportFile.open(QIODevice::ReadOnly)) {
        Logger::LogError("Failed to open export file for bundling: " + exportFilePath.toStdString());
        throw std::runtime_error("Failed to open export file for bundling.");
    }

    QString exportedData = QString::fromUtf8(exportFile.readAll());
    exportFile.close();

    QString hash = generateMD5Hash(exportedData);

    QJsonObject bundle;
    bundle[DATA_KEY] = exportedData;
    bundle[MD5_KEY] = hash;

    QFile bundleFile(bundleFilePath);
    if (!bundleFile.open(QIODevice::WriteOnly)) {
        Logger::LogError("Failed to create bundle file: " + bundleFilePath.toStdString());
        throw std::runtime_error("Failed to create bundle file.");
    }

    QJsonDocument doc(bundle);
    bundleFile.write(doc.toJson(QJsonDocument::Indented));
    bundleFile.close();
}

bool ImportExportScreenController::unbundleData(const QString &bundleFilePath, QString &exportedData, QString &hash) const
{
    QFile bundleFile(bundleFilePath);
    if (!bundleFile.open(QIODevice::ReadOnly)) {
        Logger::LogError("Failed to open bundle file: " + bundleFilePath.toStdString());
        return false;
    }

    QByteArray bundleContent = bundleFile.readAll();
    bundleFile.close();

    QJsonDocument doc = QJsonDocument::fromJson(bundleContent);
    if (!doc.isObject()) {
        Logger::LogError("Invalid bundle file format.");
        return false;
    }

    QJsonObject bundle = doc.object();
    if (!bundle.contains(DATA_KEY) || !bundle.contains(MD5_KEY)) {
        Logger::LogError("Bundle file is missing required fields.");
        return false;
    }

    exportedData = bundle[DATA_KEY].toString();
    hash = bundle[MD5_KEY].toString();

    return true;
}

void ImportExportScreenController::exportData(const QString &filePath)
{
    if (filePath.isEmpty()) {
        Logger::LogWarn("Export canceled by user.");
        emit operationStatus("Export canceled by user.");
        return;
    }

    QString adjustedFilePath = QUrl(filePath).toLocalFile();
    if (!adjustedFilePath.endsWith(".json", Qt::CaseInsensitive)) {
        adjustedFilePath += ".json";
    }

    QFile file(adjustedFilePath);
    if (!file.open(QIODevice::WriteOnly)) {
        Logger::LogError("Failed to export data to " + adjustedFilePath.toStdString());
        emit operationStatus("Failed to export data.");
        return;
    }

    QJsonObject root;
    root[EXPORT_TIMESTAMP_KEY] = QDateTime::currentDateTime().toString(Qt::ISODate);
    root[DATABASE_VERSION_KEY] = m_settingsRepository->getDatabaseVersion();
    root[COCKTAILS_KEY] = m_cocktailRepository->exportAsJson();
    root[DISCOUNTS_KEY] = m_discountsRepository->exportAsJson();
    root[GENERAL_SETTINGS_KEY] = m_settingsRepository->exportAsJson();

    QJsonDocument doc(root);
    QTextStream stream(&file);
    stream << doc.toJson(QJsonDocument::Indented);
    file.close();

    QString bundleFilePath = adjustedFilePath + ".bundle";
    try {
        bundleData(adjustedFilePath, bundleFilePath);

        if (QFile::remove(adjustedFilePath)) {
            Logger::LogInfo("Temporary export file deleted: " + adjustedFilePath.toStdString());
        } else {
            Logger::LogWarn("Failed to delete temporary export file: " + adjustedFilePath.toStdString());
        }

        Logger::LogInfo("Data exported successfully to " + bundleFilePath.toStdString());
        emit operationStatus("Export successful!");
    } catch (const std::runtime_error &e) {
        Logger::LogError(e.what());
        emit operationStatus("Export failed: " + QString(e.what()));
    }
}

void ImportExportScreenController::importData(const QString &filePath)
{
    if (filePath.isEmpty()) {
        Logger::LogWarn("Import canceled by user.");
        emit operationStatus("Import canceled by user.");
        return;
    }

    QString adjustedFilePath = QUrl(filePath).toLocalFile();
    QString exportedData, hash;

    if (!unbundleData(adjustedFilePath, exportedData, hash)) {
        emit operationStatus("Failed to unbundle data.");
        return;
    }

    if (!verifyMD5Hash(exportedData, hash)) {
        Logger::LogError("MD5 hash verification failed.");
        emit operationStatus("MD5 hash verification failed.");
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(exportedData.toUtf8());
    if (!doc.isObject()) {
        Logger::LogError("Invalid JSON format.");
        emit operationStatus("Invalid JSON format.");
        return;
    }

    QJsonObject root = doc.object();

    if (root.contains(DATABASE_VERSION_KEY)) {
        int importedVersion = root[DATABASE_VERSION_KEY].toInt();
        int currentVersion = m_settingsRepository->getDatabaseVersion();

        if (importedVersion != currentVersion) {
            Logger::LogError("Database version mismatch. Import aborted.");
            emit operationStatus("Database version mismatch. Import aborted.");
            return;
        }
    } else {
        Logger::LogError("No database version found in the import file.");
        emit operationStatus("No database version found in the import file.");
        return;
    }

    if (root.contains(COCKTAILS_KEY) && root[COCKTAILS_KEY].isArray()) {
        m_cocktailRepository->import(root[COCKTAILS_KEY].toArray());
    }

    if (root.contains(DISCOUNTS_KEY) && root[DISCOUNTS_KEY].isArray()) {
        m_discountsRepository->import(root[DISCOUNTS_KEY].toArray());
    }

    if (root.contains(GENERAL_SETTINGS_KEY) && root[GENERAL_SETTINGS_KEY].isObject()) {
        m_settingsRepository->import(root[GENERAL_SETTINGS_KEY].toObject());
    }

    Logger::LogInfo("Data imported successfully from " + adjustedFilePath.toStdString());
    emit operationStatus("Import successful!");
}
