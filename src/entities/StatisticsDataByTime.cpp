#include "StatisticsDataByTime.h"

QString StatisticsDataByTime::getTimePeriod() const
{
    return timePeriod;
}

int StatisticsDataByTime::getQuantitySold() const
{
    return quantitySold;
}

void StatisticsDataByTime::setTimePeriod(const QString &timePeriod)
{
    this->timePeriod = timePeriod;
}

void StatisticsDataByTime::setQuantitySold(int quantity)
{
    this->quantitySold = quantity;
}
