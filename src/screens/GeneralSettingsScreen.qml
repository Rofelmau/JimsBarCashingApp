import QtQuick 2.15
import QtQuick.Controls 2.15

import "qrc:/components"

Item {
    Column {
        spacing: 10

        Button {
            text: "Zurück"
            // TODO ICON hinterlegen icon.source: "qrc:/icons/left-arrow.png" // Pfad zu Ihrem Pfeil-Icon
            onClicked: {
                stackView.pop()
            }
        }

        Row {
            spacing: 10
            Text {
                text: "Preis pro Cocktail: " + GeneralSettingsScreenController.pricePerCocktail.toFixed(2) + "€"
                MouseArea {
                    anchors.fill: parent
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

        Row {
            spacing: 10
            Text {
                text: "Becherpfand: " + GeneralSettingsScreenController.cupPawn.toFixed(2) + "€"
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
            delegate: Row {
                spacing: 10
                Text {
                    text: "Selected Cocktail " + (index + 1) + ": " + GeneralSettingsScreenController["selectedCocktail" + (index + 1)]
                
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
