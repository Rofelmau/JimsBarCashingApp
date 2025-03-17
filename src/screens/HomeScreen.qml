import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "qrc:/components"

Page {
    id: homeScreen

    title: "Home Screen"

    GridLayout {
        anchors.centerIn: parent
        columns: 3
        rowSpacing: 20
        columnSpacing: 20

        // TileButton will work fine now since it's inside a GridLayout
        TileButton {

            Layout.preferredWidth: 200
            Layout.preferredHeight: 200
            Layout.alignment: Qt.AlignCenter

            buttonText: "Settings"
            borderColor: "darkblue"
            gradientStart: "#5e81f4"
            gradientEnd: "#4c66d4"
            onClickedAction: function() {
                stackView.push("SettingsScreen.qml")
            }
        }

        TileButton {

            Layout.preferredWidth: 200
            Layout.preferredHeight: 200
            Layout.alignment: Qt.AlignCenter

            buttonText: "Checkout"
            borderColor: "darkgreen"
            gradientStart: "#8bc34a"
            gradientEnd: "#689f38"
            onClickedAction: function() {
                stackView.push("CheckoutScreen.qml")
            }
        }

        TileButton {

            Layout.preferredWidth: 200
            Layout.preferredHeight: 200
            Layout.alignment: Qt.AlignCenter

            buttonText: "Statistics"
            borderColor: "darkred"
            gradientStart: "#e57373"
            gradientEnd: "#d32f2f"
            onClickedAction: function() {
                // TODO stackView.push("StatisticsScreen.qml")
            }
        }
    }
}
