import QtQuick
import canqml
import constants
import Qt5Compat.GraphicalEffects

Item {
    anchors.fill: parent
    property int speed: CanQML.FSM_LeftWheelSpeed*1000*Constants.rpmToSpeed
    property int speedRatio: (60/CanQML.FSM_LeftWheelSpeed*1000*Constants.rpmToSpeed)*5

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
        id: speedRing
        anchors.centerIn: speedString
        height: speedRatio + 200
        width: speedRatio + 200
        radius: (speedRatio + 200)/2
        border.color: "#36FB61"
        color: "transparent"
        border.width: 20
    }

    Glow {
         // set visible true when mark is hit
         anchors.fill: speedRing
         radius: 10
         samples: 20
         color: "#36FB61"
         source: speedRing
    }

}