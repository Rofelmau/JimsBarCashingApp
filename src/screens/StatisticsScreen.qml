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
        anchors.margins: 20

        Button {
            id: backButton
            text: "Zurück"
            Layout.preferredHeight: implicitHeight
            onClicked: stackView.pop()
        }

        RowLayout {
            spacing: 10
            Layout.preferredHeight: implicitHeight

            Button {
                id: startDateButton
                text: startDatePopup.selectedDate || Qt.formatDate(new Date(), "dd.MM.yyyy")
                onClicked: startDatePopup.open()
            }

            Button {
                id: endDateButton
                text: endDatePopup.selectedDate || Qt.formatDate(new Date(), "dd.MM.yyyy")
                onClicked: endDatePopup.open()
            }

            Button {
                text: "Load Statistics"
                onClicked: StatisticsScreenController.fetchCocktailSalesByDate(
                    startDatePopup.selectedDate,
                    endDatePopup.selectedDate
                )
            }
        }

        ColumnLayout {
            spacing: 5
            Layout.preferredHeight: implicitHeight

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

        Item {
            id: barChartItem
            width: parent.width
            height: implicitHeight
            Layout.preferredHeight: 0
            Layout.minimumHeight: 290

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
    }

    DatePickerPopup {
        id: startDatePopup
        onSelectedDateChanged: startDateButton.text = selectedDate
    }

    DatePickerPopup {
        id: endDatePopup
        onSelectedDateChanged: endDateButton.text = selectedDate
    }
}