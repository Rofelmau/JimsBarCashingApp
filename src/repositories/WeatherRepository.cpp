#include "WeatherRepository.h"

#include <QDate>
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

QList<WeatherData> WeatherRepository::fetchWeatherDataByTime(const QString &startDate, const QString &endDate)
{
    QList<WeatherData> weatherDataList;
    QSqlQuery query(m_dbManager->database());
    query.prepare(R"(
        SELECT strftime('%Y-%m-%d %H:00', timestamp) AS hourPeriod,
               temperature,
               condition
        FROM Weather
        WHERE timestamp BETWEEN ? AND ?
        GROUP BY hourPeriod
        ORDER BY hourPeriod ASC
    )");

    QDate startQDate = QDate::fromString(startDate, "yyyy-MM-dd");
    QDate endQDate = QDate::fromString(endDate, "yyyy-MM-dd");

    QDateTime startDateTime = startQDate.startOfDay(Qt::UTC);
    QDateTime endDateTime = endQDate.endOfDay(Qt::UTC);

    query.addBindValue(startDateTime.toString(Qt::ISODate));
    query.addBindValue(endDateTime.toString(Qt::ISODate));

    if (query.exec()) {
        while (query.next()) {
            const QString hourPeriod = query.value("hourPeriod").toString();
            const TemperatureCategory temperature = static_cast<TemperatureCategory>(query.value("temperature").toInt());
            const WeatherCondition condition = static_cast<WeatherCondition>(query.value("condition").toInt());
            weatherDataList.append(WeatherData(condition, temperature, hourPeriod));
        }
    }
    return weatherDataList;
}
