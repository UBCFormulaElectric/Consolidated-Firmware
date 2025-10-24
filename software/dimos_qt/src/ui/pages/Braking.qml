import QtQuick
import canqml
import constants
import components
import Qt5Compat.GraphicalEffects

Item {
    anchors.fill: parent
    id: innerBrake

    property int speed: CanQML.FSM_LeftWheelSpeed
    property int targetSpeed: 60
    property real speedRatio: speed / targetSpeed

    // ring size rendering
    property int defaultSize: 280
    property int ringExplosionOffset: 0
    readonly property int maxRingExplosionOffset: 600
    property bool latched: false

    // TODO remove after debug
    Keys.onSpacePressed: speed++
    Keys.onTabPressed: speed--

    // I see dead people
    PropertyAnimation {
        target: innerBrake
        properties: "ringExplosionOffset"
        id: targetReached
        running: false
        from: 0
        to: maxRingExplosionOffset
        duration: 2000
        easing.type: Easing.OutExpo
    }

    onSpeedChanged: {
        if (speed < 0) {
            speed = 0
        }
        if (speed >= targetSpeed && !latched) {
            targetReached.start()
            latched = true
        } else if (speed <= 5) {
            latched = false
        }
    }

    onLatchedChanged: {
        if (!latched) {
            targetReached.running = false
            ringExplosionOffset = 0
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

        opacity: speedRatio - ringExplosionOffset / (maxRingExplosionOffset - 100)

        size: defaultSize + (1 - speedRatio) * 200 + ringExplosionOffset
        borderWidth: 5
        colorFraction: clamp(speedRatio, 0, 1)
    }
}