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

    ColumnLayout {
        anchors.fill: parent
        spacing: 20

        Button {
            id: backButton
            text: "Zurück"
            Layout.preferredHeight: implicitHeight
            onClicked: stackView.pop()
        }

        Text {
            text: controller.hasWeatherData
                ? "Letztes Wetter: " + controller.lastWeatherCondition + ", " + controller.lastTemperature
                : "Kein Wetter festgelegt. Bitte jetzt festlegen."
            font.pointSize: 16
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
        }

        Button {
            text: controller.hasWeatherData ? "Wetter aktualisieren" : "Wetter festlegen"
            onClicked: setWeatherDialog.visible = true
        }

        SetWeatherComponent {
            id: setWeatherDialog
        }
    }
}
