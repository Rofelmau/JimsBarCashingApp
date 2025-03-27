import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.calendar 1.0

import "qrc:/components"

Page {
    id: statisticsScreen

    title: "Statistics Screen"

    ColumnLayout {
        spacing: 10
        anchors.fill: parent
        anchors.margins: 10

        Button {
            id: backButton
            text: "Zurück"
            Layout.preferredHeight: implicitHeight
            onClicked: stackView.pop()
        }

        RowLayout {
            spacing: 5
            Layout.preferredHeight: implicitHeight
            Text {
                text: "Auswertungszeitraum:"
            }
            Button {
                id: startDateButton
                text: startDatePopup.selectedDate || Qt.formatDate(new Date(), "dd.MM.yyyy")
                onClicked: startDatePopup.open()
            }
            Text {
                text: "-"
            }
            Button {
                id: endDateButton
                text: endDatePopup.selectedDate || Qt.formatDate(new Date(), "dd.MM.yyyy")
                onClicked: endDatePopup.open()
            }
            Text {
                id: dateRangeWarning
                text: "Zeitraum ungültig!"
                color: "red"
                visible: false
            }

            Item { Layout.fillWidth: true }

            Button {
                id: setEvaluationDateForTodayButton
                text: "Heute"
                onClicked: {
                    let today = new Date();
                    startDatePopup.selectedDate = Qt.formatDate(today, "dd.MM.yyyy");
                    endDatePopup.selectedDate = Qt.formatDate(today, "dd.MM.yyyy");
                }
            }
            Button {
                id: setEvaluationDateForThisWeekButton
                text: "Diese Woche"
                onClicked: {
                    let today = new Date();
                    let dayOfWeek = today.getDay();
                    let lastMonday = new Date(today);
                    lastMonday.setDate(today.getDate() - (dayOfWeek === 0 ? 6 : dayOfWeek - 1));
                    let nextSunday = new Date(today);
                    nextSunday.setDate(today.getDate() + (dayOfWeek === 0 ? 0 : 7 - dayOfWeek));

                    startDatePopup.selectedDate = Qt.formatDate(lastMonday, "dd.MM.yyyy");
                    endDatePopup.selectedDate = Qt.formatDate(nextSunday, "dd.MM.yyyy");
                }
            }
            Button {
                id: setEvaluationDateForThisMonthButton
                text: "Dieser Monat"
                onClicked: {
                    let today = new Date();
                    let firstDayOfMonth = new Date(today.getFullYear(), today.getMonth(), 1);
                    let lastDayOfMonth = new Date(today.getFullYear(), today.getMonth() + 1, 0);

                    startDatePopup.selectedDate = Qt.formatDate(firstDayOfMonth, "dd.MM.yyyy");
                    endDatePopup.selectedDate = Qt.formatDate(lastDayOfMonth, "dd.MM.yyyy");
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: implicitHeight

            ColumnLayout {
                spacing: 5
                Layout.preferredHeight: implicitHeight
                Layout.fillWidth: true

                Text {
                    text: "Gesamtumsatz: €" + StatisticsScreenController.totalRevenue.toFixed(2)
                    font.bold: true
                }
                Text {
                    text: "Anzahl verkaufter Cocktails: " + StatisticsScreenController.totalCocktailsSold
                    font.bold: true
                }
                Text {
                    text: "Bestseller: " + StatisticsScreenController.bestSellerCocktail
                    font.bold: true
                }
            }

            Item { Layout.fillWidth: true }

            Button {
                id: toggleChartButton

                Layout.alignment: Qt.AlignBottom | Qt.AlignRight

                readonly property string lineChartText: "Verkäufe nach Zeit"
                readonly property string barChartText: "Verkäufe pro Cocktail"

                text: barChartItem.visible ? lineChartText : barChartText
                onClicked: {
                    barChartItem.visible = !barChartItem.visible;
                    lineChartItem.visible = !lineChartItem.visible;
                    toggleChartButton.text = barChartItem.visible ? lineChartText : barChartText;
                }
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumHeight: 290

            Item {
                id: barChartItem
                anchors.fill: parent
                visible: true

                property var salesData: StatisticsScreenController.salesData

                Rectangle {
                    anchors.fill: parent
                    color: "lightgray"
                    border.color: "red"
                    border.width: 2
                }

                Canvas {
                    id: barChartCanvas
                    anchors.fill: parent

                    onPaint: {
                        let ctx = barChartCanvas.getContext("2d");
                        ctx.clearRect(0, 0, barChartCanvas.width, barChartCanvas.height);

                        let barWidth = 40;
                        let barSpacing = 30;
                        let chartHeight = barChartCanvas.height - 80;
                        let maxBarHeight = chartHeight - 10;
                        let maxQuantity = 0;

                        let data = barChartCanvas.parent.salesData;

                        if (data.length === 0) return;

                        for (let i = 0; i < data.length; i++) {
                            maxQuantity = Math.max(maxQuantity, data[i].quantitySold);
                        }

                        for (let i = 0; i < data.length; i++) {
                            let entry = data[i];
                            let barHeight = (entry.quantitySold / maxQuantity) * maxBarHeight;
                            let x = i * (barWidth + barSpacing) + 60;
                            let y = chartHeight - barHeight;

                            ctx.fillStyle = "#3498db";
                            ctx.fillRect(x, y, barWidth, barHeight);

                            ctx.fillStyle = "#fff";
                            ctx.font = "14px sans-serif";
                            ctx.textAlign = "center";
                            ctx.fillText(entry.quantitySold, x + barWidth / 2, y + barHeight / 2);

                            ctx.fillStyle = "#000";
                            ctx.font = "11px sans-serif";

                            let label = entry.cocktailName;
                            let words = label.split(" ");
                            let lines = [];
                            let currentLine = "";

                            for (let word of words) {
                                if ((currentLine + word).length <= 10) {
                                    currentLine += (currentLine ? " " : "") + word;
                                } else {
                                    lines.push(currentLine);
                                    currentLine = word;
                                }
                            }
                            if (currentLine) lines.push(currentLine);

                            ctx.save();
                            ctx.translate(x + barWidth / 2, chartHeight + 30);
                            ctx.rotate(-Math.PI / 4);
                            for (let j = 0; j < lines.length; j++) {
                                ctx.fillText(lines[j], 0, j * 12);
                            }
                            ctx.restore();
                        }

                        ctx.strokeStyle = "#000";
                        ctx.lineWidth = 1;

                        ctx.beginPath();
                        ctx.moveTo(40, chartHeight);
                        ctx.lineTo(barChartCanvas.width - 10, chartHeight);
                        ctx.stroke();

                        ctx.beginPath();
                        ctx.moveTo(40, 0);
                        ctx.lineTo(40, chartHeight);
                        ctx.stroke();
                    }

                    Component.onCompleted: barChartCanvas.requestPaint()
                }

                Connections {
                    target: StatisticsScreenController
                    function onSalesDataChanged() {
                        barChartCanvas.requestPaint();
                    }
                }
            }

            Item {
                id: lineChartItem
                anchors.fill: parent
                visible: false

                property var salesData: StatisticsScreenController.salesDataByTime
                property var weatherData: StatisticsScreenController.weatherDataByTime

                Rectangle {
                    anchors.fill: parent
                    color: "lightgray"
                    border.color: "blue"
                    border.width: 2
                }

                Canvas {
                    id: lineChartCanvas
                    anchors.fill: parent

                    onPaint: {
                        let ctx = lineChartCanvas.getContext("2d");
                        ctx.clearRect(0, 0, lineChartCanvas.width, lineChartCanvas.height);

                        let chartWidth = lineChartCanvas.width - 100;
                        let chartHeight = lineChartCanvas.height - 100;
                        let maxQuantity = 0;

                        let salesData = lineChartCanvas.parent.salesData;
                        let weatherData = lineChartCanvas.parent.weatherData;

                        if (salesData.length === 0 && weatherData.length === 0) return;

                        let allTimestamps = [...new Set(
                            salesData.map(d => d.timePeriod).concat(weatherData.map(d => d.timePeriod))
                        )].sort();

                        if (allTimestamps.length === 0) return;

                        for (let i = 0; i < salesData.length; i++) {
                            maxQuantity = Math.max(maxQuantity, salesData[i].quantitySold);
                        }

                        if (maxQuantity === 0) return;

                        let xPadding = 20;
                        let xSpacing = allTimestamps.length > 1 ? (chartWidth - xPadding) / (allTimestamps.length - 1) : chartWidth;
                        let yScale = chartHeight / maxQuantity;

                        ctx.strokeStyle = "#3498db";
                        ctx.lineWidth = 2;
                        ctx.beginPath();

                        for (let i = 0; i < allTimestamps.length; i++) {
                            let timestamp = allTimestamps[i];
                            let salesEntry = salesData.find(d => d.timePeriod === timestamp);
                            if (salesEntry) {
                                let x = 40 + xPadding + i * xSpacing;
                                let y = chartHeight - salesEntry.quantitySold * yScale + 40;

                                if (i === 0 || !salesData.find(d => d.timePeriod === allTimestamps[i - 1])) {
                                    ctx.moveTo(x, y);
                                } else {
                                    ctx.lineTo(x, y);
                                }

                                ctx.fillStyle = "#3498db";
                                ctx.beginPath();
                                ctx.arc(x, y, 4, 0, 2 * Math.PI);
                                ctx.fill();

                                ctx.fillStyle = "#000";
                                ctx.font = "12px sans-serif";
                                ctx.textAlign = "center";
                                ctx.fillText(salesEntry.quantitySold, x, y - 10);
                            }
                        }
                        ctx.stroke();

                        for (let i = 0; i < allTimestamps.length; i++) {
                            let timestamp = allTimestamps[i];
                            let weatherEntry = weatherData.find(d => d.timePeriod === timestamp);
                            let x = 40 + xPadding + i * xSpacing;

                            if (weatherEntry) {
                                ctx.fillStyle = "#000";
                                ctx.font = "10px sans-serif";
                                ctx.textAlign = "center";
                                ctx.fillText(timestamp, x, chartHeight + 65);

                                ctx.fillText(weatherEntry.temperature, x, chartHeight + 80);
                                ctx.fillText(weatherEntry.condition, x, chartHeight + 95);
                            }
                        }

                        ctx.strokeStyle = "#000";
                        ctx.lineWidth = 1;

                        ctx.beginPath();
                        ctx.moveTo(40, chartHeight + 40);
                        ctx.lineTo(lineChartCanvas.width - 10, chartHeight + 40);
                        ctx.stroke();

                        ctx.beginPath();
                        ctx.moveTo(40, 40);
                        ctx.lineTo(40, chartHeight + 40);
                        ctx.stroke();
                    }

                    Component.onCompleted: lineChartCanvas.requestPaint()
                }

                Connections {
                    target: StatisticsScreenController
                    function onSalesDataByTimeChanged() {
                        lineChartCanvas.requestPaint();
                    }
                    function onWeatherDataByTimeChanged() {
                        lineChartCanvas.requestPaint();
                    }
                }
            }
        }
    }

    Connections {
        target: StatisticsScreenController
        function onInvalidDateRange() {
            dateRangeWarning.visible = true;
        }
    }

    DatePickerPopup {
        id: startDatePopup
        onSelectedDateChanged: {
            startDateButton.text = selectedDate;
            dateRangeWarning.visible = false;
            StatisticsScreenController.fetchCocktailSales(startDatePopup.selectedDate, endDatePopup.selectedDate);
        }
    }

    DatePickerPopup {
        id: endDatePopup
        onSelectedDateChanged: {
            endDateButton.text = selectedDate;
            dateRangeWarning.visible = false;
            StatisticsScreenController.fetchCocktailSales(startDatePopup.selectedDate, endDatePopup.selectedDate);
        }
    }
}