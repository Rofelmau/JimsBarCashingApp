import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 800
    height: 480

    StackView {
        id: stackView
        initialItem: "qrc:/screens/HomeScreen.qml"
        anchors.fill: parent
    }
}