import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Page {
    id: cocktailsConfigurationScreen
    title: "Cocktails Configuration"

    ColumnLayout {
        anchors.fill: parent
        spacing: 20

        RowLayout {
            Layout.alignment: Qt.AlignLeft

            Button {
                text: "Zurück"
                // TODO ICON hinterlegen icon.source: "qrc:/icons/left-arrow.png" // Pfad zu Ihrem Pfeil-Icon
                onClicked: {
                    stackView.pop()
                }
            }


            Button {
                text: "Cocktail hinzufügen"
                onClicked: {
                    addCocktailDialog.open()
                }
            }
        }

        ListView {
            id: cocktailsListView
            width: parent.width
            height: parent.height * 0.75
            model: CocktailsConfigurationScreenController.cocktails
            spacing: 10

            delegate: Item {
                width: cocktailsListView.width
                height: 100

                Column {
                    spacing: 5

                    Row {
                        spacing: 10

                        Text {
                            text: modelData.name
                            width: parent.width * 0.6
                        }

                        Button {
                            text: "Bearbeiten"
                            onClicked: {
                                editCocktailDialog.cocktailId = modelData.uuid;
                                editCocktailDialog.cocktailName = modelData.name;
                                editCocktailDialog.cocktailIngredients = modelData.ingredients.slice();
                                editCocktailDialog.open();
                            }
                        }

                        Button {
                            text: "Löschen"
                            onClicked: {
                                deleteCocktailDialog.cocktailId = modelData.uuid;
                                deleteCocktailDialog.cocktailName = modelData.name;
                                deleteCocktailDialog.open();
                            }
                        }
                    }

                    Text {
                        text: "Zutaten: " + modelData.ingredients.join(", ")
                        width: parent.width
                    }
                }
            }

            ScrollBar.vertical: ScrollBar {
                policy: ScrollBar.AsNeeded
            }
        }
    }

    Dialog {
        id: addCocktailDialog
        width: parent.width * 0.8
        height: parent.height * 0.8
        modal: true

        property var newCocktailIngredients: []

        onOpened: {
            newCocktailNameField.text = "";
            addCocktailDialog.newCocktailIngredients = [];
            newIngredientsListView.model = addCocktailDialog.newCocktailIngredients.slice();

            addCocktailDialog.x = (parent.width - addCocktailDialog.width) / 2;
            addCocktailDialog.y = (parent.height - addCocktailDialog.height) / 2;
        }

        ColumnLayout {
            anchors.fill: parent
            spacing: 5

            TextField {
                id: newCocktailNameField
                placeholderText: "Cocktail namen eingeben ..."
                width: parent.width * 0.8
                onTextChanged: saveButton.enabled = text !== ""
            }

            ListView {
                id: newIngredientsListView

                Layout.fillWidth: true
                Layout.fillHeight: true
                model: addCocktailDialog.newCocktailIngredients

                spacing: 20

                delegate: Item {
                    width: parent.width
                    height: 40

                    Column {
                        spacing: 5

                        Row {
                            spacing: 10

                            Text {
                                text: modelData
                                width: parent.width * 0.75
                                height: parent.height
                                verticalAlignment: Text.AlignVCenter
                            }

                            Button {
                                text: "Entfernen"
                                onClicked: {
                                    addCocktailDialog.newCocktailIngredients.splice(index, 1)
                                    newIngredientsListView.model = addCocktailDialog.newCocktailIngredients.slice()
                                }
                            }
                        }

                        Rectangle {
                            width: parent.width
                            height: 1
                            color: "lightgray"
                        }
                    }
                }

                ScrollBar.vertical: ScrollBar {
                    policy: ScrollBar.AlwaysOn
                }
            }
            
            RowLayout {
                Layout.fillWidth: true
                height: 40

                TextField {
                    id: newIngredientField
                    placeholderText: "Zutat eingeben ..."
                    Layout.fillWidth: true
                }

                Button {
                    text: "Zutat hinzufügen"
                    enabled: newIngredientField.text !== ""
                    onClicked: {
                        addCocktailDialog.newCocktailIngredients.push(newIngredientField.text)
                        newIngredientField.clear()
                        newIngredientsListView.model = addCocktailDialog.newCocktailIngredients.slice()
                        saveButton.enabled = newCocktailNameField.text !== ""
                    }
                }
            }

            RowLayout {
                Layout.fillWidth: true
                height: 40

                Button {
                    text: "Abbrechen"
                    onClicked: {
                        newCocktailNameField.clear()
                        addCocktailDialog.newCocktailIngredients = []
                        addCocktailDialog.close()
                    }
                }

                Button {
                    id: saveButton
                    text: "Speichern"
                    enabled: false
                    onClicked: {
                        CocktailsConfigurationScreenController.addNewCocktail(newCocktailNameField.text, addCocktailDialog.newCocktailIngredients)
                        newCocktailNameField.clear()
                        addCocktailDialog.newCocktailIngredients = []
                        addCocktailDialog.close()
                    }
                }
            }
        }
    }

    Dialog {
        id: editCocktailDialog
        width: parent.width * 0.8
        height: parent.height * 0.8
        modal: true

        property string cocktailId
        property string cocktailName
        property var cocktailIngredients

        onOpened: {
            editCocktailNameField.text = editCocktailDialog.cocktailName;
            editCocktailDialog.cocktailIngredients = editCocktailDialog.cocktailIngredients.slice();
            editIngredientsListView.model = editCocktailDialog.cocktailIngredients.slice();

            editCocktailDialog.x = (parent.width - editCocktailDialog.width) / 2;
            editCocktailDialog.y = (parent.height - editCocktailDialog.height) / 2;
        }

        ColumnLayout {
            anchors.fill: parent
            spacing: 5

            TextField {
                id: editCocktailNameField
                Layout.fillWidth: true
                height: 40
                text: editCocktailDialog.cocktailName
                placeholderText: "Cocktailnamen eingeben ..."
                onTextChanged: editSaveButton.enabled = text !== ""
            }

            ListView {
                id: editIngredientsListView

                Layout.fillWidth: true
                Layout.fillHeight: true
                model: editCocktailDialog.cocktailIngredients

                spacing: 20

                delegate: Item {
                    width: parent.width
                    height: 40

                    Row {
                        spacing: 10

                        Text {
                            text: modelData
                            width: parent.width * 0.75
                            height: parent.height
                            verticalAlignment: Text.AlignVCenter
                        }

                        Button {
                            text: "Entfernen"
                            onClicked: {
                                editCocktailDialog.cocktailIngredients.splice(index, 1);
                                editIngredientsListView.model = editCocktailDialog.cocktailIngredients.slice();
                            }
                        }
                    }
                }

                ScrollBar.vertical: ScrollBar {
                    policy: ScrollBar.AlwaysOn
                }
            }

            RowLayout {
                Layout.fillWidth: true
                height: 40

                TextField {
                    id: newEditIngredientField
                    placeholderText: "Zutat eingeben ..."
                    Layout.fillWidth: true
                }

                Button {
                    text: "Zutat hinzufügen"
                    enabled: newEditIngredientField.text !== ""
                    onClicked: {
                        if (!editCocktailDialog.cocktailIngredients.includes(newEditIngredientField.text)) {
                            editCocktailDialog.cocktailIngredients.push(newEditIngredientField.text);
                            newEditIngredientField.clear();
                            editIngredientsListView.model = editCocktailDialog.cocktailIngredients.slice();
                            editSaveButton.enabled = editCocktailNameField.text !== ""
                        }
                    }
                }
            }

            RowLayout {
                Layout.fillWidth: true
                height: 40

                Button {
                    text: "Abbrechen"
                    onClicked: {
                        editCocktailDialog.close();
                    }
                }

                Button {
                    id: editSaveButton
                    text: "Speichern"
                    enabled: false
                    onClicked: {
                        CocktailsConfigurationScreenController.editCocktail(
                            editCocktailDialog.cocktailId,
                            editCocktailNameField.text,
                            editCocktailDialog.cocktailIngredients.slice()
                        );
                        editCocktailDialog.close();
                    }
                }
            }
        }
    }
 
    Dialog {
        id: deleteCocktailDialog
        modal: true

        property string cocktailId
        property string cocktailName

        onOpened: {
            deleteCocktailDialog.x = (parent.width - deleteCocktailDialog.width) / 2
            deleteCocktailDialog.y = (parent.height - deleteCocktailDialog.height) / 2
        }

        ColumnLayout {
            spacing: 10
            width: parent.width

            Label {
                text: "Bist du dir sicher, dass du den Cocktail löschen möchtest: " + deleteCocktailDialog.cocktailName + "?"
                width: parent.width * 0.8
            }

            RowLayout {
                Button {
                    text: "Abbrechen"
                    onClicked: {
                        deleteCocktailDialog.close()
                    }
                }

                Button {
                    text: "Löschen"
                    onClicked: {
                        CocktailsConfigurationScreenController.deleteCocktail(deleteCocktailDialog.cocktailId)
                        deleteCocktailDialog.close()
                    }
                }
            }
        }
    }
}