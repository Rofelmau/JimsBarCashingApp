import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "qrc:/components"

Page {
    id: discountsConfigurationScreen

    title: "Rabatte verwalten"

    AddDiscountDialog {
        id: addDiscountDialog
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: implicitHeight

            Button {
                text: "Zurück"
                // TODO ICON hinterlegen icon.source: "qrc:/icons/left-arrow.png" // Pfad zu Ihrem Pfeil-Icon
                onClicked: {
                    stackView.pop()
                }
            }

            Item { Layout.fillWidth: true } // Flexible element to make the add discount button stick to the right edge fo the screen

            Button {
                text: "Neuen Rabatt hinzufügen"
                onClicked: addDiscountDialog.openForAdd()
            }
        }

        RowLayout {
            id: tableHeaders

            Layout.fillWidth: true
            spacing: 10

            Text {
                id: nameColumnHeadline

                text: "Name"
                font.bold: true
                Layout.preferredWidth: 125
            }

            Text {
                id: typeColumnHeadline

                text: "Typ"
                font.bold: true
                Layout.preferredWidth: 60

            }

            Text {
                id: valueColumnHeadline

                text: "Wert"
                font.bold: true
            }

            Text {
                id: limitColumnHeadline

                text: "Limit"
                font.bold: true
                Layout.preferredWidth: 70
            }

            Text {
                id: reusableColumnHeadline

                text: "Wiederverwendbar"
                font.bold: true
            }

            Text {
                id: combinableColumnHeadline

                text: "Kombinierbar"
                font.bold: true
            }

            Text {
                id: aktionColumnHeadline

                text: ""
                font.bold: true
                Layout.preferredWidth: 200
            }
        }

        ListView {
            id: discountListView

            Layout.fillWidth: true
            Layout.fillHeight: true

            model: DiscountsConfigurationScreenController.discountModel

            delegate: Item {
                width: discountListView.width
                height: 50

                RowLayout {
                    spacing: 10

                    Text {
                        text: name
                        Layout.preferredWidth: nameColumnHeadline.width
                    }

                    Text {
                        text: type
                        Layout.preferredWidth: typeColumnHeadline.width
                    }

                    Text {
                        text: value
                        Layout.preferredWidth: valueColumnHeadline.width
                    }

                    Text {
                        text: cocktailLimit > 0 ? cocktailLimit : "Kein Limit"
                        Layout.preferredWidth: limitColumnHeadline.width
                    }

                    Text {
                        text: reusable ? "Ja" : "Nein"
                        Layout.preferredWidth: reusableColumnHeadline.width
                    }

                    Text {
                        text: combinable ? "Ja" : "Nein"
                        Layout.preferredWidth: combinableColumnHeadline.width
                    }

                    RowLayout {
                        spacing: 5
                        Layout.preferredWidth: aktionColumnHeadline.width

                        Button {
                            text: "Bearbeiten"
                            onClicked: {
                                addDiscountDialog.openForEdit(model)
                            }
                        }

                        Button {
                            text: "Löschen"
                            onClicked: DiscountsConfigurationScreenController.deleteDiscount(id)
                        }
                    }
                }
            }
        }
    }
}