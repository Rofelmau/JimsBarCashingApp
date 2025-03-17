import QtQuick 2.15
import QtQuick.Controls 2.15

Dialog {
    id: selectCocktailDialog
    title: dialogTitle
    modal: true
    standardButtons: DialogButtonBox.Save | DialogButtonBox.Cancel

    height: parent.height * 0.5
    width: parent.width * 0.5

    property alias dialogTitle: titleText.text
    property var availableCocktails: []
    property var onSave: null
    property int selectedCocktailId: -1

    onOpened: {
        selectCocktailDialog.x = (parent.width - selectCocktailDialog.width) / 2
        selectCocktailDialog.y = (parent.height - selectCocktailDialog.height) / 2

        // Add "None" option and sort cocktails alphabetically
        let cocktailsWithNone = availableCocktails.slice();
        cocktailsWithNone.push({ id: -1, name: "None" });
        availableCocktails = cocktailsWithNone.sort((a, b) => a.name.localeCompare(b.name));

        // Preselect the current cocktail ID if available
        for (var i = 0; i < availableCocktails.length; i++) {
            if (availableCocktails[i].id === selectedCocktailId) {
                cocktailComboBox.currentIndex = i;
                break;
            }
        }
    }

    Column {
        spacing: 10
        anchors.fill: parent
        anchors.margins: 20

        Text {
            text: "Select a Cocktail:"
            font.pixelSize: 16
            horizontalAlignment: Text.AlignHCenter
            anchors.horizontalCenter: parent.horizontalCenter
        }

        ComboBox {
            id: cocktailComboBox
            width: parent.width * 0.9
            anchors.horizontalCenter: parent.horizontalCenter
            model: availableCocktails
            textRole: "name"
            valueRole: "id"

            onCurrentIndexChanged: {
                if (currentIndex >= 0) {
                    selectCocktailDialog.selectedCocktailId = availableCocktails[currentIndex].id;
                }
            }
        }
    }

    onAccepted: {
        if (onSave) {
            onSave(selectedCocktailId);
        }
    }

    Text {
        id: titleText
        visible: false
    }
}
