import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQml 2.15

import App.Helpers 1.0

Dialog {
    id: addDiscountDialog
    modal: true
    property bool isEditMode: false
    property string discountUuid: ""

    title: isEditMode ? "Rabatt bearbeiten" : "Neuen Rabatt hinzufügen"

    onOpened: {
        addDiscountDialog.x = (parent.width - addDiscountDialog.width) / 2
        addDiscountDialog.y = (parent.height - addDiscountDialog.height) / 2
    }

    DiscountTypeHelper {
        id: discountTypeHelper
    }

    ColumnLayout {
        spacing: 10
        width: parent.width

        Text {
            text: "Name des Rabatts"
            Layout.fillWidth: true
        }
        TextField {
            id: nameField
            placeholderText: "Name des Rabatts"
            Layout.fillWidth: true
        }

        Rectangle {
            Layout.fillWidth: true
            height: 5
        }

        Text {
            text: "Rabatt Type"
            Layout.fillWidth: true
        }
        ComboBox {
            id: typeComboBox
            Layout.fillWidth: true
            model: discountTypeHelper.values()
            currentIndex: 0
            onCurrentIndexChanged: {
                const selectedType = typeComboBox.currentIndex;
                valueField.enabled = selectedType !== DiscountTypeHelper.ForFree;
                cocktailLimitField.enabled = selectedType === DiscountTypeHelper.GroupDiscount;
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 5
        }

        Text {
            text: "Wert des Rabatts"
            Layout.fillWidth: true
        }
        TextField {
            id: valueField
            placeholderText: "Wert des Rabatts"
            inputMethodHints: Qt.ImhFormattedNumbersOnly
            Layout.fillWidth: true
            enabled: typeComboBox.currentIndex !== DiscountTypeHelper.ForFree
        }

        Rectangle {
            Layout.fillWidth: true
            height: 5
        }

        Text {
            text: "Cocktail-Menge"
            Layout.fillWidth: true
        }
        TextField {
            id: cocktailLimitField
            placeholderText: "Cocktail-Menge"
            inputMethodHints: Qt.ImhFormattedNumbersOnly
            Layout.fillWidth: true
            enabled: typeComboBox.currentIndex === DiscountTypeHelper.GroupDiscount
        }

        Rectangle {
            Layout.fillWidth: true
            height: 5
        }

        RowLayout {
            spacing: 10
            Layout.alignment: Qt.AlignRight

            Button {
                text: "Abbrechen"
                onClicked: addDiscountDialog.close()
            }

            Button {
                text: isEditMode ? "Speichern" : "Hinzufügen"
                enabled: nameField.text.length > 0 && (valueField.enabled ? valueField.text.length > 0 : true)
                onClicked: {
                    if (isEditMode) {
                        DiscountsConfigurationScreenController.updateDiscount(
                            discountUuid,
                            nameField.text,
                            typeComboBox.currentIndex,
                            parseFloat(valueField.text),
                            cocktailLimitField.text.length > 0 ? parseInt(cocktailLimitField.text) : -1
                        );
                    } else {
                        DiscountsConfigurationScreenController.addDiscount(
                            nameField.text,
                            typeComboBox.currentIndex,
                            parseFloat(valueField.text),
                            cocktailLimitField.text.length > 0 ? parseInt(cocktailLimitField.text) : -1
                        );
                    }
                    addDiscountDialog.close();
                }
            }
        }
    }

    function openForEdit(discount) {
        isEditMode = true;
        discountUuid = discount.discountId;
        nameField.text = discount.name;
        typeComboBox.currentIndex = discount.typeValue;
        valueField.text = discount.value.toString();
        cocktailLimitField.text = discount.cocktailLimit;

        const selectedType = discount.typeValue;
        valueField.enabled = selectedType !== DiscountTypeHelper.ForFree;
        cocktailLimitField.enabled = selectedType === DiscountTypeHelper.GroupDiscount;

        open();
    }

    function openForAdd() {
        isEditMode = false;
        discountUuid = "";
        nameField.text = "";
        typeComboBox.currentIndex = 0;
        valueField.text = "-1.0";
        cocktailLimitField.text = "-1";

        valueField.enabled = true;
        cocktailLimitField.enabled = false;

        open();
    }
}
