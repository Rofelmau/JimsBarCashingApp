import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "qrc:/components"

Page {
    id: weatherSettingsScreen

    property var controller: WeatherSettingsScreenController

    Component.onCompleted: {
        controller.fetchLatestWeather();
    }

    Button {
        id: backButton
        text: "Zurück"
        onClicked: stackView.pop()
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 20

        Text {
            text: controller.hasWeatherData
                ? "Letztes Wetter: " + controller.lastWeatherCondition + ", " + controller.lastTemperature
                : "Kein Wetter festgelegt. Bitte jetzt festlegen."
            font.pointSize: 16
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            Layout.alignment: Qt.AlignHCenter
        }

        Button {
            text: controller.hasWeatherData ? "Wetter aktualisieren" : "Wetter festlegen"
            onClicked: setWeatherDialog.visible = true
            Layout.alignment: Qt.AlignHCenter
        }

        SetWeatherComponent {
            id: setWeatherDialog
        }
    }
}
