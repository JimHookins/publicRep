import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15

ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: qsTr("Test")

    header: ToolBar {
        id: toolBar

        height: 40

        ToolButton {
            id: updateButton

            anchors.verticalCenter: parent.verticalCenter
            height: parent.height
            width: 130
            text: "Update model"

            onClicked: rootContext.updateModel()
        }
    }


    ListView {
        anchors.fill: parent
        anchors.margins: 5
        spacing: 5

        model: DataModel

        delegate: DelegateItem {
            text:  id + " " + name

            onClicked: {
                name = "Clicked"
                console.log("Clicked " + text)
            }
        }
    }
}
