import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import JimsBarCashingApp 1.0
import App.Helpers 1.0

Dialog {
    id: setWeatherDialog
    modal: true
    visible: false
    width: 600
    height: 350

    property var controller: SetWeatherComponentController
    property string errorMessage: ""

    property var manualSelectedWeatherCondition
    property var manualSelecteTemperatureCategory

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

    onOpened: {
        setWeatherDialog.x = (parent.width - setWeatherDialog.width) / 2
        setWeatherDialog.y = (parent.height - setWeatherDialog.height) / 2
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 16
        anchors.margins: 5

        BusyIndicator {
            Layout.alignment: Qt.AlignHCenter
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
            visible: !controller.isLoading
            text: "Wetter setzen:"
            font.bold: true
        }

        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            visible: !controller.isLoading
            spacing: 8

            Repeater {
                model: weatherConditionHelper.values()
                Rectangle {
                    width: 100
                    height: 100
                    color: "transparent"
                    border.color: "transparent"

                    Button {
                        anchors.fill: parent
                        text: weatherConditionHelper.weatherConditionToString(modelData)
                        onClicked: controller.setManualWeatherCondition(modelData)
                        checked: controller.weatherCondition === modelData
                        checkable: true
                    }
                }
            }
        }

        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            visible: !controller.isLoading
            spacing: 8

            Repeater {
                model: temperatureCategoryHelper.values()
                Rectangle {
                    width: 100
                    height: 100
                    color: "transparent"
                    border.color: "transparent"

                    Button {
                        anchors.fill: parent
                        text: temperatureCategoryHelper.temperatureCategoryToString(modelData)
                        onClicked: controller.setManualTemperatureCategory(modelData)
                        checked: controller.temperatureCategory === modelData
                        checkable: true
                    }
                }
            }
        }

        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            spacing: 8

            Button {
                text: "Wetter erneut abrufen"
                enabled: !controller.isLoading
                onClicked: controller.fetchWeatherForLocation()
            }

            Button {
                text: "Abbrechen"
                onClicked: setWeatherDialog.close()
            }

            Button {
                text: "Speichern"
                visible: !controller.isLoading
                onClicked: {
                    controller.saveWeatherData();
                    setWeatherDialog.close();
                }
            }
        }
    }

    Connections {
        target: controller
        function onFetchFailed(message) {
            errorMessage = message;
        }
    }
}
