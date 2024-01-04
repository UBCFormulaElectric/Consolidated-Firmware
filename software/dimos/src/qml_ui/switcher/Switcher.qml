import QtQuick
import squircle

Item {
    anchors.fill: parent

    Squircle {
        anchors.centerIn: parent
        radius: 40
        smoothness: 0.3
        width: 200
        height: 200
        color: "#013372"
        Image {
            source: "qrc:/SwitcherEventIcons/Endurance.svg"
            anchors.centerIn: parent
            sourceSize.width: parent.width
            sourceSize.height: parent.height
            width: parent.width * 0.7
            height: parent.height
            fillMode: Image.PreserveAspectFit
        }
    }
}