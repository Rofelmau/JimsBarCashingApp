import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "qrc:/components"

Item {
    ColumnLayout {
        spacing: 10

        anchors.fill: parent
        anchors.margins: 5

        Button {
            text: "Zurück"
            // TODO ICON hinterlegen icon.source: "qrc:/icons/left-arrow.png" // Pfad zu Ihrem Pfeil-Icon
            onClicked: {
                stackView.pop()
            }
        }

        RowLayout {
            spacing: 10
            Text {
                text: "Preis pro Cocktail: "
                font.pixelSize: 20
                color: "black"
            }

            Rectangle {
                width: 100
                height: 40
                border.color: "black"
                border.width: 1
                color: "lightgray"

                Text {
                    anchors.centerIn: parent
                    text:  GeneralSettingsScreenController.pricePerCocktail.toFixed(2) + "€"
                    font.pixelSize: 20
                    color: "black"

                    MouseArea {
                        anchors.fill: parent
                        width: parent.width + 20
                        height: parent.height + 20
                        onClicked: {
                            changeSettingsDialog.dialogTitle = "Preis pro Cocktail";
                            changeSettingsDialog.initialValue = GeneralSettingsScreenController.pricePerCocktail.toFixed(2);
                            changeSettingsDialog.onSave = function(value) {
                                GeneralSettingsScreenController.savePricePerCocktail(value);
                            };
                            changeSettingsDialog.open();
                        }
                    }
                }
            }
        }

        RowLayout {
            spacing: 10
            Text {
                text: "Becherpfand:"
                font.pixelSize: 20
                color: "black"
            }

            Rectangle {
                width: 100
                height: 40
                border.color: "black"
                border.width: 1
                color: "lightgray"

                Text {
                    anchors.centerIn: parent
                    text: GeneralSettingsScreenController.cupPawn.toFixed(2) + "€"
                    font.pixelSize: 20
                    color: "black"
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        changeSettingsDialog.dialogTitle = "Becherpfand";
                        changeSettingsDialog.initialValue = GeneralSettingsScreenController.cupPawn.toFixed(2);
                        changeSettingsDialog.onSave = function(value) {
                            GeneralSettingsScreenController.saveCupPawn(value);
                        };
                        changeSettingsDialog.open();
                    }
                }
            }
        }

        Repeater {
            model: 6
            delegate: RowLayout {
                spacing: 10
                Text {
                    text: "Selected Cocktail " + (index + 1) + ":"
                    font.pixelSize: 20
                    color: "black"
                }

                Rectangle {
                    width: 200
                    height: 40
                    border.color: "black"
                    border.width: 1
                    color: "lightgray"

                    Text {
                        anchors.centerIn: parent
                        text: GeneralSettingsScreenController["selectedCocktail" + (index + 1)]
                        font.pixelSize: 20
                        color: "black"
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            selectCocktailDialog.dialogTitle = "Select Cocktail " + (index + 1);
                            selectCocktailDialog.availableCocktails = GeneralSettingsScreenController.availableCocktails;
                            selectCocktailDialog.selectedCocktailUuid = GeneralSettingsScreenController["selectedCocktail" + (index + 1) + "Uuid"];
                            selectCocktailDialog.onSave = function(cocktailUuid) {
                                GeneralSettingsScreenController.setSelectedCocktail(index + 1, cocktailUuid);
                            };
                            selectCocktailDialog.open();
                        }
                    }
                }
            }
        }
    }

    ChangeSettingsDialog {
        id: changeSettingsDialog
    }

    SelectCocktailDialog {
        id: selectCocktailDialog
    }
}
