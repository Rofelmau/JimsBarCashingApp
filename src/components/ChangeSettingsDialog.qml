import QtQuick 2.15
import QtQuick.Controls 2.15

Dialog {
    id: changeSettingsDialog
    title: dialogTitle
    modal: true
    standardButtons: DialogButtonBox.Save | DialogButtonBox.Cancel

    height: parent.height * 0.35
    width: parent.width * 0.4

    property alias dialogTitle: titleText.text
    property string initialValue: ""
    property var onSave: null

    onOpened: {
        changeSettingsDialog.x = (parent.width - changeSettingsDialog.width) / 2
        changeSettingsDialog.y = (parent.height - changeSettingsDialog.height) / 2
    }

    Column {
        spacing: 10
        TextField {
            id: inputField
            text: initialValue
            inputMethodHints: Qt.ImhFormattedNumbersOnly
        }
    }

    onAccepted: {
        if (onSave) {
            onSave(parseFloat(inputField.text));
        }
    }

    Text {
        id: titleText
        visible: false
    }
}
