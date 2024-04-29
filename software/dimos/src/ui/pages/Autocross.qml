import QtQuick
import QtQuick.Shapes
import components
import canqml
import constants

Item {
    anchors.fill: parent
    property real stateOfCharge: CanQML.BMS_Soc;

    onActiveFocusChanged: {
        if (activeFocus) {
            acclerationSpeedometer.startup_animation_running = true
        }
    }

    Speedometer {
        id: acclerationSpeedometer
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        startup_animation_running: false
    }

    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: acclerationSpeedometer.bottom
        anchors.topMargin: 30
        font.family: "Roboto"
        font.pointSize: 24
        text: "Uwu Points: " + stateOfCharge
        color: "#ffffff"
    }
}