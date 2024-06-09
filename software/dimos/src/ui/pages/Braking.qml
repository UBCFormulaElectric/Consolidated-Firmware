import QtQuick
import canqml
import constants
import components
import Qt5Compat.GraphicalEffects

Item {
    anchors.fill: parent

    property int speed: CanQML.FSM_LeftWheelSpeed
    property int targetSpeed: 60
    property real speedRatio: speed / targetSpeed

    // ring size rendering
    property int defaultSize: 260
    property int ringExtra: 0

    // TODO remove after debug
    Keys.onSpacePressed: speed++
    Keys.onTabPressed: speed--

    // I see dead people
    PropertyAnimation {
        target: outerRing
        properties: "ringExplosionOffset"
        id: targetReached
        running: false
        from: 0
        to: 600
        duration: 3000
        easing.type: Easing.OutExpo
    }

    onSpeedChanged: {
        if (speed < 0) {
            speed = 0
        }
        if (speed >= targetSpeed) {
            targetReached.start()
        }
    }

    function clamp(value, min, max) {
        return Math.max(min, Math.min(max, value))
    }

    Text {
        id: speedString
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -5
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
        borderWidth: 28
        colorFraction: clamp(speedRatio, 0, 1)
    }

    // Outer ring (moves)
    BrakeRing {
        id: outerRing
        visible: true
        anchors.horizontalCenter: speedString.horizontalCenter
        anchors.verticalCenter: speedString.verticalCenter

        property int ringExplosionOffset: 0
        size: defaultSize + (1 - speedRatio) * 200 + ringExplosionOffset
        borderWidth: 5
        colorFraction: clamp(speedRatio, 0, 1)
    }
}