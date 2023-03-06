import QtQuick 2.15
import QtQuick.Controls 2.15

ItemDelegate {
    id: root

    implicitWidth: 300
    implicitHeight: 50

    background: Rectangle {
        anchors.fill: parent
        color: "#EAF2F7"
        border {
            color: "#2E3B4C"
            width: root.hovered ? 2 : 1
        }
        radius: 5
    }
}
