import QtQuick
import QtQuick.Shapes
import components
import canqml

Item {
    anchors.fill: parent

    id: innerAutocross

    onActiveFocusChanged: {
        if (activeFocus) {
            startupAnimation.running = true
        }
    }

    Text {
        id: speedString
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: acclerationSpeedometer.top
        anchors.topMargin: 70
        // anchors.top: parent.top
        // anchors.topMargin: 200
        font.family: "Roboto"
        font.bold: true
        font.pointSize: 72
        text: CanQML.FSM_LeftWheelSpeed // do math for wheel
        color: "#ffffff"
    }

    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: speedString.bottom
        anchors.topMargin: -5
        font.family: "Roboto"
        font.pointSize: 14
        text: "km/h"
        color: "#868686"
    }

    CircleProgressBar {
        id: acclerationSpeedometer
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        // anchors.top: parent.top
        // anchors.topMargin: 135
        width: 250
        height: 250
        property real speed_percentage: CanQML.FSM_LeftWheelSpeed / 150;
        percentage: speed_percentage;
        stroke_width: 8
        start_angle: 270
        end_angle: -90
        round_tip: true
        bar_color: "#ffffff"

        background: true
        bg_percentage: 1
        bg_color: "#696969"

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
}