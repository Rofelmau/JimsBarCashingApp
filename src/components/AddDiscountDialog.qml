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
        }

        TextField {
            id: valueField
            placeholderText: "Wert des Rabatts"
            inputMethodHints: Qt.ImhFormattedNumbersOnly
            Layout.fillWidth: true
        }

        TextField {
            id: cocktailLimitField
            placeholderText: "Cocktail-Limit (optional)"
            inputMethodHints: Qt.ImhFormattedNumbersOnly
            Layout.fillWidth: true
        }

        CheckBox {
            id: reusableCheckBox
            text: "Mehrfach anwendbar"
            checked: false
        }

        CheckBox {
            id: combinableCheckBox
            text: "Kombinierbar"
            checked: true
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
                enabled: nameField.text.length > 0 && valueField.text.length > 0
                onClicked: {
                    if (isEditMode) {
                        DiscountsConfigurationScreenController.updateDiscount(
                            discountId,
                            nameField.text,
                            typeComboBox.currentText,
                            parseFloat(valueField.text),
                            cocktailLimitField.text.length > 0 ? parseInt(cocktailLimitField.text) : -1,
                            reusableCheckBox.checked,
                            combinableCheckBox.checked
                        )
                    } else {
                        DiscountsConfigurationScreenController.addDiscount(
                            nameField.text,
                            typeComboBox.currentText,
                            parseFloat(valueField.text),
                            cocktailLimitField.text.length > 0 ? parseInt(cocktailLimitField.text) : -1,
                            reusableCheckBox.checked,
                            combinableCheckBox.checked
                        )
                    }
                    addDiscountDialog.close()
                }
            }
        }
    }

    function openForEdit(discount) {
        isEditMode = true;
        discountId = discount.id;
        nameField.text = discount.name;

        const discountValues = discountTypeHelper.values();
        typeComboBox.currentIndex = discountValues.indexOf(discount.type);

        valueField.text = discount.value.toString();
        cocktailLimitField.text = discount.cocktailLimit > 0 ? discount.cocktailLimit.toString() : "";
        reusableCheckBox.checked = discount.reusable;
        combinableCheckBox.checked = discount.combinable;
        open();
    }

    function openForAdd() {
        isEditMode = false;
        discountId = -1;
        nameField.text = "";
        typeComboBox.currentIndex = 0;
        valueField.text = "";
        cocktailLimitField.text = "";
        reusableCheckBox.checked = false;
        combinableCheckBox.checked = true;
        open();
    }
}
