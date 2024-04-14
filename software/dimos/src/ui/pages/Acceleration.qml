import QtQuick
import QtQuick.Shapes
import components
import canqml

Item {
    anchors.fill: parent
    id: innerAcceleration
    property double rpmToSpeed: 3.1415926*60/39370

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
        text: parseInt(CanQML.FSM_LeftWheelSpeed*1000*rpmToSpeed) // do math for wheel
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
        property real speed_percentage: (parseInt(CanQML.FSM_LeftWheelSpeed*1000*rpmToSpeed)) / 150;
        percentage: speed_percentage;
        stroke_width: 18
        start_angle: 225
        end_angle: -45
        round_tip: true
        bar_color: "#36FB61"

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

    CircleProgressBar {
        id: leftBar
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        // anchors.top: parent.top
        // anchors.topMargin: 135
        width: 325
        height: 325
        percentage: 0.50
        stroke_width: 15
        start_angle: 240
        end_angle: 160
        round_tip: true
        bar_color: "#FBDC36"

        background: true
        bg_color: "#696969"
        bg_percentage: 1
    }

    Text {
        anchors.left: leftBar.horizontalCenter
        anchors.leftMargin: -65
        anchors.top: leftBar.bottom
        anchors.topMargin: -30
        font.family: "Roboto"
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
        width: 325
        height: 325
        percentage: 0.50
        stroke_width: 15
        start_angle: -60
        end_angle: 20
        round_tip: true
        bar_color: "#FBDC36"

        background: true
        bg_color: "#696969"
        bg_percentage: 1
    }

    Text {
        anchors.right: rightBar.horizontalCenter
        anchors.rightMargin: -65
        anchors.top: rightBar.bottom
        anchors.topMargin: -30
        font.family: "Roboto"
        font.pointSize: 12
        font.bold: true
        text: "B"
        color: "#FFFFFF"
    }
}