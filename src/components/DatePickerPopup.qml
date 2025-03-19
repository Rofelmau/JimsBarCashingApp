import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.calendar 1.0

Popup {
    id: datePickerPopup
    property string selectedDate: Qt.formatDate(new Date(), "dd.MM.yyyy")
    property alias gridId: monthGrid

    width: 350
    height: 250
    modal: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    onOpened: {
        datePickerPopup.x = (parent.width - datePickerPopup.width) / 2
        datePickerPopup.y = (parent.height - datePickerPopup.height) / 2
    }

    Column {
        spacing: 10
        anchors.fill: parent

        Row {
            spacing: 10
            anchors.horizontalCenter: parent.horizontalCenter

            Button {
                text: "<"
                onClicked: {
                    if (monthGrid.month === 0) {
                        monthGrid.month = 11;
                        monthGrid.year -= 1;
                    } else {
                        monthGrid.month -= 1;
                    }
                }
            }

            Text {
                text: Qt.formatDate(new Date(monthGrid.year, monthGrid.month), "MMMM yyyy")
                font.bold: true
            }

            Button {
                text: ">"
                onClicked: {
                    if (monthGrid.month === 11) {
                        monthGrid.month = 0;
                        monthGrid.year += 1;
                    } else {
                        monthGrid.month += 1;
                    }
                }
            }
        }

        GridLayout {
            columns: 2
            anchors.horizontalCenter: parent.horizontalCenter

            DayOfWeekRow {
                locale: monthGrid.locale
                Layout.column: 1
                Layout.fillWidth: true
            }

            WeekNumberColumn {
                month: monthGrid.month
                year: monthGrid.year
                locale: monthGrid.locale
                Layout.fillHeight: true
            }

            MonthGrid {
                id: monthGrid
                month: new Date().getMonth()
                year: new Date().getFullYear()
                locale: Qt.locale("de_DE")
                Layout.fillWidth: true
                Layout.fillHeight: true

                onClicked: {
                    datePickerPopup.selectedDate = Qt.formatDate(date, "dd.MM.yyyy")
                    datePickerPopup.close()
                }
            }
        }
    }
}
