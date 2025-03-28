import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3

Page {
    title: "Import/Export"

    FileDialog {
        id: exportFileDialog
        title: "Save Exported Data"
        selectExisting: false
        nameFilters: ["JSON Files (*.json)", "All Files (*)"]
        onAccepted: ImportExportScreenController.exportData(fileUrl.toString())
        onRejected: console.log("Export canceled by user.")
    }

    FileDialog {
        id: importFileDialog
        title: "Select File to Import"
        selectExisting: true
        nameFilters: ["Bundle Files (*.bundle)", "All Files (*)"] // Unterstützt jetzt .bundle-Dateien
        onAccepted: ImportExportScreenController.importData(fileUrl.toString())
        onRejected: console.log("Import canceled by user.")
    }

    MessageDialog {
        id: messageDialog

        title: "Import/Export"
        text: ""
    }

    Connections {
        target: ImportExportScreenController
        function onOperationStatus(message) {
            messageDialog.text = message;
            messageDialog.open();
        }
    }

    Column {
        spacing: 20
        anchors.centerIn: parent

        Button {
            text: "Export Data"
            onClicked: exportFileDialog.open()
        }

        Button {
            text: "Import Data"
            onClicked: importFileDialog.open()
        }
    }
}
