#include "WeatherRepository.h"
#include <QSqlQuery>
#include <QVariant>

WeatherRepository::WeatherRepository(QSharedPointer<DatabaseManager> dbManager)
    : QObject(nullptr)
    , m_dbManager(dbManager)
{
}

bool WeatherRepository::saveWeatherData(const WeatherData &data)
{
    QSqlQuery query(m_dbManager->database());
    query.prepare("INSERT INTO Weather (condition, temperature, timestamp) VALUES (?, ?, ?)");
    query.addBindValue(static_cast<int>(data.getCondition()));
    query.addBindValue(static_cast<int>(data.getTemperature()));
    query.addBindValue(data.getTimestamp());
    if (query.exec()) {
        emit weatherDataUpdated();
        return true;
    }
    return false;
}

QList<WeatherData> WeatherRepository::fetchWeatherData()
{
    QList<WeatherData> weatherDataList;
    QSqlQuery query(m_dbManager->database());
    query.prepare("SELECT condition, temperature, timestamp FROM Weather ORDER BY timestamp DESC");

    if (query.exec()) {
        while (query.next()) {
            WeatherCondition condition = static_cast<WeatherCondition>(query.value(0).toInt());
            TemperatureCategory temperature = static_cast<TemperatureCategory>(query.value(1).toInt());
            QString timestamp = query.value(2).toString();
            weatherDataList.append(WeatherData(condition, temperature, timestamp));
        }
    }
    return weatherDataList;
}
