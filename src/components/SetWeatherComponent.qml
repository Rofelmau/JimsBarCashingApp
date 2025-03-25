import QtQuick 2.15
import QtQuick.Controls 2.15
import JimsBarCashingApp 1.0

import App.Helpers 1.0

Dialog {
    id: setWeatherDialog
    modal: true
    visible: false

    property var controller: SetWeatherComponentController
    property string errorMessage: ""

    TemperatureCategoryHelper {
        id: temperatureCategoryHelper
    }

    WeatherConditionHelper {
        id: weatherConditionHelper
    }

    onVisibleChanged: {
        if (visible) {
            errorMessage = "";
            controller.fetchWeatherForLocation();
        }
    }

    Column {
        spacing: 16
        padding: 16

        Button {
            text: "Wetter abrufen"
            enabled: !controller.isLoading
            onClicked: controller.fetchWeatherForLocation()
        }

        BusyIndicator {
            visible: controller.isLoading
            running: controller.isLoading
        }

        Text {
            visible: errorMessage !== ""
            text: errorMessage
            color: "red"
            wrapMode: Text.WordWrap
        }

        Text {
            visible: !controller.isLoading && errorMessage === ""
            text: "Wetter: " + weatherConditionHelper.weatherConditionToString(controller.weatherCondition) +
                  "\nTemperatur: " + temperatureCategoryHelper.temperatureCategoryToString(controller.temperatureCategory)
            wrapMode: Text.WordWrap
        }

        Button {
            text: "Speichern"
            visible: !controller.isLoading && errorMessage === ""
            onClicked: {
                controller.saveWeatherData();
                setWeatherDialog.close();
            }
        }

        Button {
            text: "Abbrechen"
            onClicked: setWeatherDialog.close()
        }
    }

    Connections {
        target: controller
        function onFetchFailed(message) {
            errorMessage = message;
        }
    }
}
