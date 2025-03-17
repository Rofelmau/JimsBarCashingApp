import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.15

Button {
    id: tileButton

    property string buttonText: ""
    property color borderColor: "darkblue"
    property color gradientStart: "#5e81f4"
    property color gradientEnd: "#4c66d4"
    property var onClickedAction: null

    width: 200
    height: 200

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
        border.color: tileButton.borderColor
        border.width: 2

        gradient: Gradient {
            GradientStop { position: 0.0; color: tileButton.gradientStart }
            GradientStop { position: 1.0; color: tileButton.gradientEnd }
        }

        Text {
            anchors.centerIn: parent
            text: tileButton.buttonText
            font.pixelSize: 18
            font.bold: true
            color: "white"
            wrapMode: Text.Wrap
            horizontalAlignment: Text.AlignHCenter // Horizontale Zentrierung des Textes
            width: parent.width * 0.8 // Optional, damit der Text nicht den gesamten Button ausfüllt
        }
    }

    onClicked: {
        if (tileButton.onClickedAction) {
            tileButton.onClickedAction()
        }
    }
}
