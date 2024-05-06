import QtQuick
import canqml
import constants
import components
import Qt5Compat.GraphicalEffects

Item {
    anchors.fill: parent

    property int speed: CanQML.FSM_LeftWheelSpeed*1000*Constants.rpmToSpeed
    property int targetSpeed: 60
    property real speedRatio: speed/targetSpeed
    property int defaultSize: 200

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

    // Inner ring
    BrakeRing {
        anchors.horizontalCenter: speedString.horizontalCenter
        anchors.verticalCenter: speedString.verticalCenter
        anchors.verticalCenterOffset: 10
        size: defaultSize
        colorFraction: speedRatio
    }

    // Outer ring (moves)
    BrakeRing {
        anchors.horizontalCenter: speedString.horizontalCenter
        anchors.verticalCenter: speedString.verticalCenter
        anchors.verticalCenterOffset: 10
        size: defaultSize + speedRatio * 100
        colorFraction: speedRatio
    }

    // Debug
    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: speedString.bottom
        anchors.topMargin: 30
        font.family: "Roboto"
        font.pointSize: 16
        text: speedRatio
        color: "#ffffff"
    }

}