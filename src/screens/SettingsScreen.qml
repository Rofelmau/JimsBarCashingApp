import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "qrc:/components"

Page {
    id: homeScreen

    title: "Home Screen"

    ColumnLayout {
        anchors.fill: parent
        spacing: 5

        RowLayout {
            Layout.alignment: Qt.AlignLeft

            Button {
                text: "Zurück"
                // TODO ICON hinterlegen icon.source: "qrc:/icons/left-arrow.png" // Pfad zu Ihrem Pfeil-Icon
                onClicked: {
                    stackView.pop()
                }
            }
        }

        GridLayout {
            Layout.alignment: Qt.AlignHCenter
            columns: 3
            rowSpacing: 20
            columnSpacing: 20

            TileButton {
                Layout.preferredWidth: 200
                Layout.preferredHeight: 200
                Layout.alignment: Qt.AlignCenter

                buttonText: "Allgemeine Einstellungen"
                borderColor: "darkred"
                gradientStart: "#e57373"
                gradientEnd: "#d32f2f"
                onClickedAction: function() {
                    stackView.push("GeneralSettingsScreen.qml")
                }
            }

            TileButton {
                Layout.preferredWidth: 200
                Layout.preferredHeight: 200
                Layout.alignment: Qt.AlignCenter

                buttonText: "Cocktails Verwalten"
                borderColor: "darkblue"
                gradientStart: "#5e81f4"
                gradientEnd: "#4c66d4"
                onClickedAction: function() {
                    stackView.push("CocktailsConfigurationScreen.qml")
                }
            }

            TileButton {
                Layout.preferredWidth: 200
                Layout.preferredHeight: 200
                Layout.alignment: Qt.AlignCenter

                buttonText: "Rabatte Verwalten"
                borderColor: "darkgreen"
                gradientStart: "#8bc34a"
                gradientEnd: "#689f38"
                onClickedAction: function() {
                    stackView.push("DiscountsConfigurationScreen.qml")
                }
            }

            TileButton {
                Layout.preferredWidth: 200
                Layout.preferredHeight: 200
                Layout.alignment: Qt.AlignCenter

                buttonText: "Wetter"
                borderColor: "darkred"
                gradientStart: "#e57373"
                gradientEnd: "#d32f2f"
                onClickedAction: function() {
                    stackView.push("WeatherSettingsScreen.qml")
                }
            }

            TileButton {
                Layout.preferredWidth: 200
                Layout.preferredHeight: 200
                Layout.alignment: Qt.AlignCenter

                buttonText: "Kassenstand"
                borderColor: "darkblue"
                gradientStart: "#5e81f4"
                gradientEnd: "#4c66d4"
                onClickedAction: function() {
                    stackView.push("CashBalanceScreen.qml")
                }
            }

            TileButton {
                Layout.preferredWidth: 200
                Layout.preferredHeight: 200
                Layout.alignment: Qt.AlignCenter

                buttonText: "Import/Export"
                borderColor: "darkgreen"
                gradientStart: "#8bc34a"
                gradientEnd: "#689f38"
                onClickedAction: function() {
                    stackView.push("ImportExportScreen.qml")
                }
            }
        }
    }
}
