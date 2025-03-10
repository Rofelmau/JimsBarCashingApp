import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    visible: true
    width: 850
    height: 600
    title: "Kacheln Layout"

    RowLayout {
        anchors.fill: parent

        GridLayout {
            columns: 3
            Layout.fillWidth: true
            Layout.fillHeight: true

            Repeater {
                model: cocktailModel

                Rectangle {
                    width: 200
                    height: 200
                    color: model.color
                    radius: 10

                    function getContrastingColor(hexColor) {
                        var r = parseInt(hexColor.substr(1, 2), 16)
                        var g = parseInt(hexColor.substr(3, 2), 16)
                        var b = parseInt(hexColor.substr(5, 2), 16)
                        var brightness = (r * 299 + g * 587 + b * 114) / 1000
                        return brightness > 128 ? "black" : "white"
                    }

                    Text {
                        id: text
                        anchors.top: parent.top
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.topMargin: 10
                        text: model.name
                        color: getContrastingColor(model.color)
                        font.pixelSize: 20
                    }

                    Image {
                        width: 100
                        height: 100
                        source: model.image ? model.image : "fallback.png"
                        anchors.top: text.bottom
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.topMargin: 10
                        onStatusChanged: {
                            if (status == Image.Error) {
                                source = "fallback.png"
                            }
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            addClickedCocktail(model.name)
                        }
                    }
                }
            }

            Component.onCompleted: {
                for (var i = 0; i < cocktailModel.count; i++) {
                    var item = itemAt(i)
                    if (item) {
                        item.margins = 10 
                    }
                }
            }
        }

        ListView {
            width: 250
            Layout.fillHeight: true
            model: clickedCocktailsModel

            delegate: Item {
                width: parent.width
                height: 50

                Rectangle {
                    width: parent.width
                    height: 50
                    color: "#4CAF50"
                    radius: 5
                    border.color: "white"
                    border.width: 1

                    Text {
                        text: model.name + ": " + model.count + "x"
                        color: "white"
                        font.pixelSize: 18
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }
        }
    }

    ListModel {
        id: clickedCocktailsModel
    }

    function addClickedCocktail(name) {
        for (var i = 0; i < clickedCocktailsModel.count; i++) {
            var item = clickedCocktailsModel.get(i)
            if (item.name === name) {
                clickedCocktailsModel.setProperty(i, "count", item.count + 1)
                return
            }
        }
        clickedCocktailsModel.append({"name": name, "count": 1})
    }
}
