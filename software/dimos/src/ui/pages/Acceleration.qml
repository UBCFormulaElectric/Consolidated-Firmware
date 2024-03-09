import QtQuick
import QtQuick.Shapes

Item {
    anchors.fill: parent
    property var speed: 100 // Change to canQML

    Text {
        id: speedString
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 150
        font.family: "Roboto"
        font.bold: true
        font.pointSize: 64
        text: speed
        color: "#ffffff"
    }

    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: speedString.bottom
        anchors.topMargin: -5
        font.family: "Roboto"
        font.pointSize: 12
        text: "km/h"
        color: "#868686"
    }


    // Try to get the accerlation bit to show, try to display everything
    // Then add animations to racing base and accel
}