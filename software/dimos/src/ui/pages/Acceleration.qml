import QtQuick
import QtQuick.Shapes
import components
import canqml
import constants

Item {
    anchors.fill: parent
    id: innerAcceleration
    property real sappsPedalPercentage: CanQML.FSM_SappsMappedPedalPercentage
    property real pappsPedalPercentage: CanQML.FSM_PappsMappedPedalPercentage

    readonly property int innerDiameter: 290
    readonly property int outerDiameter: 370
    readonly property string dialBackgroundColour: "#1fffffff"

    function onPageLoad() {
        startupAnimation.running = true
    }

    Text {
        id: speedString
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -10
        font.bold: true
        font.pointSize: 80
        lineHeight: 0.8
        text: CanQML.FSM_LeftWheelSpeed
        color: "#ffffff"
    }

    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: speedString.bottom
        anchors.topMargin: -15
        font.pointSize: 14
        font.bold: true
        text: "KM/H"
        color: "#868686"
    }

    CircleProgressBar {
        id: acclerationSpeedometer
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        // anchors.top: parent.top
        // anchors.topMargin: 135
        width: innerAcceleration.innerDiameter
        height: innerAcceleration.innerDiameter
        property real speed_percentage: CanQML.FSM_LeftWheelSpeed / 150;
        percentage: speed_percentage;
        stroke_width: 28
        start_angle: 225
        end_angle: -45
        round_tip: true
        bar_color: "#36FB61"

        background: true
        bg_percentage: 1
        bg_color: innerAcceleration.dialBackgroundColour

        SequentialAnimation {
            id: startupAnimation
            running: false
            NumberAnimation {
                target: acclerationSpeedometer; property: "percentage";
                from: 0;
                to: 1; duration: 1000;
                easing.type: Easing.InOutQuad;
            }
            NumberAnimation {
                target: acclerationSpeedometer; property: "percentage";
                to: acclerationSpeedometer.speed_percentage; duration: 1000;
                easing.type: Easing.InOutQuad;
            }
        }
    }

    CircleProgressBar {
        id: leftBar
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        // anchors.top: parent.top
        // anchors.topMargin: 135
        width: innerAcceleration.outerDiameter
        height: innerAcceleration.outerDiameter
        percentage: innerAcceleration.sappsPedalPercentage / 100
        stroke_width: 18
        start_angle: 240
        end_angle: 160
        round_tip: true
        bar_color: "#e79503"

        background: true
        bg_color: innerAcceleration.dialBackgroundColour
        bg_percentage: 1
    }

    Text {
        anchors.left: leftBar.horizontalCenter
        anchors.leftMargin: -65
        anchors.top: leftBar.bottom
        anchors.topMargin: -30
        font.pointSize: 12
        font.bold: true
        text: "A"
        color: "#FFFFFF"
    }

    CircleProgressBar {
        id: rightBar
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        // anchors.top: parent.top
        // anchors.topMargin: 135
        width: innerAcceleration.outerDiameter
        height: innerAcceleration.outerDiameter
        percentage: innerAcceleration.pappsPedalPercentage / 100
        stroke_width: 18
        start_angle: -60
        end_angle: 20
        round_tip: true
        bar_color: "#e79503"

        background: true
        bg_color: innerAcceleration.dialBackgroundColour
        bg_percentage: 1
    }

    Text {
        anchors.right: rightBar.horizontalCenter
        anchors.rightMargin: -65
        anchors.top: rightBar.bottom
        anchors.topMargin: -30
        font.pointSize: 12
        font.bold: true
        text: "B"
        color: "#FFFFFF"
    }
}