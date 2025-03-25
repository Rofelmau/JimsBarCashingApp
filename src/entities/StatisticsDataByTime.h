#pragma once

#include <QString>

class StatisticsDataByTime {
public:
    QString getTimePeriod() const;
    int getQuantitySold() const;

    void setTimePeriod(const QString &timePeriod);
    void setQuantitySold(int quantity);

private:
    QString timePeriod;
    int quantitySold;
};
