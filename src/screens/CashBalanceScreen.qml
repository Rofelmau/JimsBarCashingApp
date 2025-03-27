import QtQuick 2.15
import QtQuick.Controls 2.15

Page {
    title: "Cash Balance"

    Button {
        id: backButton
        text: "Zurück"
        onClicked: stackView.pop()
    }

    Column {
        spacing: 20
        anchors.centerIn: parent

        Text {
            text: "Aktueller Kassenstand: " + CashBalanceScreenController.balance.toFixed(2) + "€"
            font.pointSize: 20
        }

        Row {
            spacing: 10

            TextField {
                id: amountField
                placeholderText: "Enter amount"
                inputMethodHints: Qt.ImhFormattedNumbersOnly
            }

            Button {
                text: "Add"
                onClicked: {
                    CashBalanceScreenController.modifyBalance(parseFloat(amountField.text), "Add money")
                }
            }

            Button {
                text: "Remove"
                onClicked: {
                    CashBalanceScreenController.modifyBalance(-parseFloat(amountField.text), "Remove money")
                }
            }
        }
    }
}
