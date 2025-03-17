#include "StatisticsScreenController.h"

#include "SalesRepository.h"

#include "Logger.h"

#include <QDate>

StatisticsScreenController::StatisticsScreenController(QSharedPointer<SalesRepository> salesRepository, QObject *parent)
    : QObject(parent), m_salesRepository(salesRepository)
{
}

void StatisticsScreenController::fetchCocktailSalesByDate(const QString &startDate, const QString &endDate)
{
    // Ensure dates are in the correct format
    QDate parsedStartDate = QDate::fromString(startDate, "dd.MM.yyyy");
    QDate parsedEndDate = QDate::fromString(endDate, "dd.MM.yyyy");

    if (!parsedStartDate.isValid() || !parsedEndDate.isValid()) {
        Logger::LogWarn("Invalid date format received: startDate=" + startDate.toStdString() + ", endDate=" + endDate.toStdString());
        return;
    }

    QString formattedStartDate = parsedStartDate.toString("yyyy-MM-dd");
    QString formattedEndDate = parsedEndDate.toString("yyyy-MM-dd");

    // Fetch sales data from the repository
    m_salesDataList = m_salesRepository->getSalesData(formattedStartDate, formattedEndDate);

    if (m_salesDataList.isEmpty()) {
        Logger::LogInfo("No sales data found for the given date range.");
    } else {
        Logger::LogInfo("Fetched sales data: " + std::to_string(m_salesDataList.size()) + " entries.");
    }

    emit salesDataChanged();
}

QVariantList StatisticsScreenController::salesData() const
{
    QVariantList result;
    for (const auto &entry : m_salesDataList) {
        QVariantMap map;
        map["cocktailName"] = entry.cocktailName;
        map["quantitySold"] = entry.quantitySold;
        map["pricePerCocktail"] = entry.pricePerCocktail;
        result.append(map);
    }
    return result;
}

double StatisticsScreenController::getTotalRevenue() const
{
    double totalRevenue = 0.0;
    for (const auto &entry : m_salesDataList) {
        totalRevenue += entry.quantitySold * entry.pricePerCocktail;
    }
    return totalRevenue;
}

int StatisticsScreenController::getTotalCocktailsSold() const
{
    int totalSold = 0;
    for (const auto &entry : m_salesDataList) {
        totalSold += entry.quantitySold;
    }
    return totalSold;
}

QString StatisticsScreenController::getBestSellerCocktail() const
{
    QString bestSeller;
    int maxSold = 0;
    for (const auto &entry : m_salesDataList) {
        if (entry.quantitySold > maxSold) {
            maxSold = entry.quantitySold;
            bestSeller = entry.cocktailName;
        }
    }
    return bestSeller;
}
