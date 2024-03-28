import QtQuick

Item {
    anchors.fill: parent
    property var speed: 60 // Change to canQML

    Text {
        id: speedString
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        // anchors.top: parent.top
        // anchors.topMargin: 200
        font.family: "Roboto"
        font.bold: true
        font.pointSize: 82
        text: speed
        color: "#ffffff"
    }

    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: speedString.bottom
        anchors.topMargin: -5
        font.family: "Roboto"
        font.pointSize: 16
        text: "km/h"
        color: "#868686"
    }

    Rectangle {
        anchors.centerIn: speedString
        height: 200
        width: 200
        radius: 100
        border.color: "#36FB61"
        color: "transparent"
        border.width: 20
    }

    Rectangle {
        anchors.centerIn: speedString
        height: 400
        width: 400
        radius: 200
        border.color: "#36FB61"
        opacity: 0.5
        color: "transparent"
        border.width: 5
    }

    Rectangle {
        anchors.centerIn: speedString
        height: 600
        width: 600
        radius: 300
        border.color: "#36FB61"
        opacity: 0.5
        color: "transparent"
        border.width: 5
    }
}