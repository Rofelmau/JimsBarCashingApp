import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQml 2.15
import QtGraphicalEffects 1.15


import "qrc:/components"

Page {
    id: cocktailsConfigurationScreen
    visible: true

    property var controller: CheckoutScreenController

    readonly property int buttonGridButtonsWidth: 135
    readonly property int buttonGridButtonTextSize: 12

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 5

        spacing: 5

        Row {
            width: parent.width
            spacing: 10

            Button {
                text: "Zurück"
                // TODO ICON hinterlegen icon.source: "qrc:/icons/left-arrow.png" // Pfad zu Ihrem Pfeil-Icon
                onClicked: {
                    stackView.pop()
                }
            }

            Rectangle {
                width: 230
                height: 40
                border.color: "black"
                border.width: 1
                color: "lightgray"

                Text {
                    id: pricePerCocktail

                    anchors.centerIn: parent

                    text: "Preis pro Cocktail: " + controller.pricePerCocktail.toFixed(2) + "€"
                    font.pointSize: 12
                }
            }

            Rectangle {
                width: 180
                height: 40
                border.color: "black"
                border.width: 1
                color: "lightgray"
                
                Text {
                    id: cupPawn

                    anchors.centerIn: parent

                    text: "Becherpfand: " + controller.cupPawn.toFixed(2) + "€"
                    font.pointSize: 12
                }
            }

            Button {
                text: "Kassenstand"

                onClicked: {
                    stackView.push("CashBalanceScreen.qml")
                }
            }
        }

        RowLayout {
            width: parent.width
            spacing: 10

            Layout.fillWidth: true
            Layout.fillHeight: true

            ColumnLayout {
                GridLayout {
                    columns: 3
                    rows: 2
                    rowSpacing: 5
                    columnSpacing: 10

                    Repeater {
                        model: controller.selectedCocktails

                        TileButton {
                            Layout.preferredWidth: buttonGridButtonsWidth
                            Layout.preferredHeight: buttonGridButtonsWidth
                            Layout.alignment: Qt.AlignCenter

                            buttonText: modelData.name
                            borderColor: "darkgreen"
                            gradientStart: "#8bc34a"
                            gradientEnd: "#689f38"
                            onClickedAction: function() {
                                controller.cocktailSelected(modelData.id);
                            }
                        }
                    }
                }

                GridLayout {
                    id: discounts
                    columns: 3
                    rows: 2
                    rowSpacing: 5
                    columnSpacing: 10

                    Repeater {
                        model: controller.availableDiscounts

                        Button {
                            id: discountButton

                            Layout.preferredWidth: buttonGridButtonsWidth
                            Layout.preferredHeight: 40

                            property color borderColor: "darkred"
                            property color gradientStart: "#e57373"
                            property color gradientEnd: "#d32f2f"
                            property var onClickedAction: null

                            DropShadow {
                                anchors.fill: parent
                                horizontalOffset: 4
                                verticalOffset: 4
                                color: "#000000"
                                radius: 8
                                samples: 16
                                source: backgroundRect
                            }

                            background: Rectangle {
                                id: backgroundRect
                                width: parent.width
                                height: parent.height
                                radius: 20
                                border.color: discountButton.borderColor
                                border.width: 2

                                gradient: Gradient {
                                    GradientStop { position: 0.0; color: discountButton.gradientStart }
                                    GradientStop { position: 1.0; color: discountButton.gradientEnd }
                                }

                                ColumnLayout {
                                    anchors.centerIn: parent
                                    spacing: 2  // Abstand zwischen den Texten

                                    Text {
                                        Layout.alignment: Qt.AlignHCenter
                                        text: modelData.name
                                        font.pixelSize: buttonGridButtonTextSize
                                        font.bold: true
                                        color: "white"
                                        wrapMode: Text.Wrap
                                        horizontalAlignment: Text.AlignHCenter
                                        Layout.fillWidth: true
                                    }

                                    Text {
                                        Layout.alignment: Qt.AlignHCenter
                                        text: modelData.infoText
                                        font.pixelSize: 8
                                        font.bold: false
                                        color: "white"
                                        wrapMode: Text.Wrap
                                        horizontalAlignment: Text.AlignHCenter
                                        Layout.fillWidth: true
                                    }
                                }
                            }

                            onClicked: {
                                controller.applyDiscount(modelData.id);
                            }
                        }
                    }
                }


                Row  {
                    width: parent.width
                    spacing: 10

                    Button {
                        id: cupPawnReturnButton

                        width: buttonGridButtonsWidth
                        height: 40

                        property string buttonText: "Pfandrückgabe"
                        property color borderColor: "darkblue"
                        property color gradientStart: "#5e81f4"
                        property color gradientEnd: "#4c66d4"
                        property var onClickedAction: null

                        DropShadow {
                            anchors.fill: parent
                            horizontalOffset: 4
                            verticalOffset: 4
                            color: "#000000"
                            radius: 8
                            samples: 16
                            source: backgroundRect
                        }

                        background: Rectangle {
                            id: backgroundRect
                            width: parent.width
                            height: parent.height
                            radius: 20
                            border.color: cupPawnReturnButton.borderColor
                            border.width: 2

                            gradient: Gradient {
                                GradientStop { position: 0.0; color: cupPawnReturnButton.gradientStart }
                                GradientStop { position: 1.0; color: cupPawnReturnButton.gradientEnd }
                            }

                            Text {
                                anchors.centerIn: parent
                                text: cupPawnReturnButton.buttonText
                                font.pixelSize: buttonGridButtonTextSize
                                font.bold: true
                                color: "white"
                                wrapMode: Text.Wrap
                                horizontalAlignment: Text.AlignHCenter // Horizontale Zentrierung des Textes
                                width: parent.width * 0.8 // Optional, damit der Text nicht den gesamten Button ausfüllt
                            }
                        }

                        onClicked: {
                            controller.returnCup();
                        }
                    }

                    Button {
                        id: cashPayButton

                        width: buttonGridButtonsWidth
                        height: 40

                        property string buttonText: "Bar Zahlung"
                        property color borderColor: "darkblue"
                        property color gradientStart: "#5e81f4"
                        property color gradientEnd: "#4c66d4"
                        property var onClickedAction: null

                        enabled: controller.totalPrice !== 0.0

                        DropShadow {
                            anchors.fill: parent
                            horizontalOffset: 4
                            verticalOffset: 4
                            color: "#000000"
                            radius: 8
                            samples: 16
                            source: backgroundRect2
                        }

                        background: Rectangle {
                            id: backgroundRect2
                            width: parent.width
                            height: parent.height
                            radius: 20
                            border.color: cashPayButton.borderColor
                            border.width: 2

                            gradient: Gradient {
                                GradientStop { position: 0.0; color: cashPayButton.gradientStart }
                                GradientStop { position: 1.0; color: cashPayButton.gradientEnd }
                            }

                            Text {
                                anchors.centerIn: parent
                                text: cashPayButton.buttonText
                                font.pixelSize: buttonGridButtonTextSize
                                font.bold: true
                                color: "white"
                                wrapMode: Text.Wrap
                                horizontalAlignment: Text.AlignHCenter // Horizontale Zentrierung des Textes
                                width: parent.width * 0.8 // Optional, damit der Text nicht den gesamten Button ausfüllt
                            }
                        }

                        onClicked: {
                            paymentDialog.paymentMethod = 0; // Cash payment
                            paymentDialog.visible = true;
                        }
                    
                    }

                    Button {
                        id: cardPaymentButton

                        width: buttonGridButtonsWidth
                        height: 40

                        property string buttonText: "Kartenzahlung"
                        property color borderColor: "darkblue"
                        property color gradientStart: "#5e81f4"
                        property color gradientEnd: "#4c66d4"
                        property var onClickedAction: null

                        enabled: controller.totalPrice > 0.0
            
                        DropShadow {
                            anchors.fill: parent
                            horizontalOffset: 4
                            verticalOffset: 4
                            color: "#000000"
                            radius: 8
                            samples: 16
                            source: backgroundRect3
                        }

                        background: Rectangle {
                            id: backgroundRect3
                            width: parent.width
                            height: parent.height
                            radius: 20
                            border.color: cardPaymentButton.borderColor
                            border.width: 2

                            gradient: Gradient {
                                GradientStop { position: 0.0; color: cardPaymentButton.gradientStart }
                                GradientStop { position: 1.0; color: cardPaymentButton.gradientEnd }
                            }

                            Text {
                                anchors.centerIn: parent
                                text: cardPaymentButton.buttonText
                                font.pixelSize: buttonGridButtonTextSize
                                font.bold: true
                                color: "white"
                                wrapMode: Text.Wrap
                                horizontalAlignment: Text.AlignHCenter // Horizontale Zentrierung des Textes
                                width: parent.width * 0.8 // Optional, damit der Text nicht den gesamten Button ausfüllt
                            }
                        }

                        onClicked: {
                            paymentDialog.paymentMethod = 1; // Card payment
                            paymentDialog.visible = true;
                        }
                    }
                }

            }

            Rectangle {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.minimumWidth: 240
                border.color: "black"
                border.width: 1

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 5
                    spacing: 5

                    Text {
                        text: "Einkaufsliste:"
                        font.bold: true
                    }

                    ListView {
                        id: shoppingCard
                        width: parent.width
                        Layout.fillHeight: true
                        Layout.preferredHeight: Math.min(contentItem.childrenRect.height, parent.height * 0.4)
                        clip: true

                        model: controller.currentSaleDetails
                        spacing: 2

                        delegate: Row {
                            spacing: 2
                            Text { text: modelData.name; width: 150 }
                            Text { text: modelData.quantity + "x"; width: 40 }
                            Button {
                                width: 40
                                height: 20
                                text: "-"
                                onClicked: controller.decreaseQuantity(modelData.id);
                            }
                        }
                    }

                    Item {
                        Layout.fillHeight: true
                        Layout.preferredHeight: 10
                    }

                    ListView {
                        id: appliedDiscountsList
                        width: parent.width
                        Layout.fillHeight: true
                        Layout.preferredHeight: Math.min(contentItem.childrenRect.height, parent.height * 0.3)
                        clip: true

                        model: controller.appliedDiscounts
                        spacing: 2

                        delegate: Row {
                            spacing: 2
                            Text { text: modelData.name; width: 150 }
                            Text { text: modelData.quantity + "x"; width: 40 }
                            Button {
                                width: 40
                                height: 20
                                text: "-"
                                onClicked: controller.decreaseDiscountQuantity(modelData.id);
                            }
                        }
                    }

                    Column {
                        Layout.fillWidth: true
                        Layout.bottomMargin: 5

                        Text {
                            text: "Gesamt: " + controller.totalPrice.toFixed(2) + "€"
                            font.bold: true
                        }
                        Text {
                            text: "Inklusive Becherpfand: " + controller.totalCupPawn.toFixed(2) + "€"
                        }
                        Text {
                            text: "Zurückgegebene Becher: " + controller.returnedCups
                        }
                    }
                }
            }

        }

    }

    Dialog {
        id: paymentDialog
        modal: true
        visible: false
        title: "Zahlung bestätigen"

        onOpened: {
            paymentDialog.x = (parent.width - paymentDialog.width) / 2
            paymentDialog.y = (parent.height - paymentDialog.height) / 2
        }

        Column {
            spacing: 10
            Text {
                text: "Gesamtpreis: " + controller.totalPrice.toFixed(2) + "€"
                font.bold: true
            }
            Row {
                spacing: 10
                Button {
                    text: "Bestätigen"
                    onClicked: {
                        paymentDialog.visible = false;
                        controller.confirmPayment(paymentDialog.paymentMethod);
                    }
                }
                Button {
                    text: "Abbrechen"
                    onClicked: {
                        paymentDialog.visible = false;
                    }
                }
            }
        }

        property int paymentMethod: -1
    }
}
