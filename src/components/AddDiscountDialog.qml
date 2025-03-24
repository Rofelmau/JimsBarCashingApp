import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQml 2.15

import App.Helpers 1.0

Dialog {
    id: addDiscountDialog
    modal: true
    property bool isEditMode: false
    property int discountId: -1

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

        TextField {
            id: nameField
            placeholderText: "Name des Rabatts"
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

        TextField {
            id: valueField
            placeholderText: "Wert des Rabatts"
            inputMethodHints: Qt.ImhFormattedNumbersOnly
            Layout.fillWidth: true
            enabled: typeComboBox.currentIndex !== DiscountTypeHelper.ForFree
        }

        TextField {
            id: cocktailLimitField
            placeholderText: "Cocktail-Limit (optional)"
            inputMethodHints: Qt.ImhFormattedNumbersOnly
            Layout.fillWidth: true
            enabled: typeComboBox.currentIndex === DiscountTypeHelper.GroupDiscount
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
                    const discountValues = discountTypeHelper.values();

                    if (isEditMode) {
                        DiscountsConfigurationScreenController.updateDiscount(
                            discountId,
                            nameField.text,
                            typeComboBox.currentIndex,
                            parseFloat(valueField.text),
                            cocktailLimitField.text.length > 0 ? parseInt(cocktailLimitField.text) : -1,
                        )
                    } else {
                        DiscountsConfigurationScreenController.addDiscount(
                            nameField.text,
                            typeComboBox.currentIndex,
                            parseFloat(valueField.text),
                            cocktailLimitField.text.length > 0 ? parseInt(cocktailLimitField.text) : -1,
                        )
                    }
                    addDiscountDialog.close();
                }
            }
        }
    }

    function openForEdit(discount) {
        isEditMode = true;
        discountId = discount.discountId;
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
        discountId = -1;
        nameField.text = "";
        typeComboBox.currentIndex = 0;
        valueField.text = "-1.0";
        cocktailLimitField.text = "-1";

        valueField.enabled = true;
        cocktailLimitField.enabled = false;

        open();
    }
}
