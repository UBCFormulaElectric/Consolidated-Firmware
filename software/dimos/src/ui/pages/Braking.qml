import QtQuick
import canqml
import constants
import components
import Qt5Compat.GraphicalEffects

Item {
    anchors.fill: parent

    property int speed: CanQML.FSM_LeftWheelSpeed*1000*Constants.rpmToSpeed
    property int targetSpeed: 60
    property real speedRatio: 1 - (speed > targetSpeed ? (2*targetSpeed - speed): speed)/targetSpeed
    property int defaultSize: 200
    // property bool brakeActivated: (speed >= targetSpeed) ? true : ((speed === 0) ? false : brakeActivated)
    property int ringExtra: 0

    Keys.onSpacePressed: speed++
    Keys.onTabPressed: {
        speed--
        console.log(speedRatio)
    }

    // I see dead people
    Timer {
        id: targetReached
        interval: 10
        running: false
        repeat: true
        onTriggered: ringExtra++
    }

    onSpeedChanged: {
        if (speed >= targetSpeed) {
            targetReached.start()
        }
        if (speed <= 0) {
            console.log("FUCK")
            ringExtra = 0
            outerRing.visible = true
        }
    }

    onRingExtraChanged: {
        if (ringExtra === 500) {
            targetReached.stop()
            outerRing.visible = false
        }
    }

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
        size: defaultSize
        colorFraction: speedRatio
    }

    // Outer ring (moves)
    BrakeRing {
        id: outerRing
        visible: true
        anchors.horizontalCenter: speedString.horizontalCenter
        anchors.verticalCenter: speedString.verticalCenter
        size: defaultSize + speedRatio * 200 + ringExtra
        colorFraction: speedRatio
    }

    // Debug
    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: speedString.bottom
        anchors.topMargin: 60
        font.family: "Roboto"
        font.pointSize: 16
        text: speedRatio
        color: "#ffffff"
    }

}