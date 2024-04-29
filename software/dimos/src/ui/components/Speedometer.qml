import QtQuick
import dimswitches
import components

Item {
    width: 350
    height: 350

    property real instantaneous_power_percentage: 0.5 // TODO bind this to TractiveSystemPower
    property real avaiable_power_percentage: 0.75 // TODO bind this to DCMPowerLimitAtFullThrottle, AvailablePower
    property real hv_battery_soc: 0.5 // TODO bind these to
    property real max_cell_temp: 0.5 // TODO bind these to

    DimSwitches {
        onF1ButtonPressed: {
            enduranceSpeedometer.instantaneous_power_percentage = Math.random()
            enduranceSpeedometer.avaiable_power_percentage = Math.random()
            enduranceSpeedometer.hv_battery_soc = Math.random()
            enduranceSpeedometer.max_cell_temp = Math.random()
        }
    }

    CircleProgressBar {
        anchors.fill: parent
        anchors.margins: 30
        round_tip: true

        percentage: enduranceSpeedometer.instantaneous_power_percentage
        start_angle: -90
        end_angle: -270
        stroke_width: 10
        bar_color: enduranceSpeedometer.instantaneous_power_percentage >= 0 ? "white" : "#0CFF41"
        Behavior on bar_color {
            ColorAnimation {
                duration: 1000
            }
        }

        bg_percentage: 0.5 + 0.5 * enduranceSpeedometer.avaiable_power_percentage
        bg_start_angle: 90
        bg_end_angle: -270
        background: true
        bg_stroke_width: 7
        bg_color: "#24FFFFFF"
    }
    CircleProgressBar {
        anchors.fill: parent
        round_tip: true

        percentage: enduranceSpeedometer.hv_battery_soc
        start_angle: -140
        end_angle: -40
        stroke_width: 15
        bar_color: "#2AAFE2"

        background: true
        bg_percentage: 1
        bg_color: "#852aafe2"
    }
    CircleProgressBar {
        anchors.fill: parent
        round_tip: true

        percentage: enduranceSpeedometer.max_cell_temp
        start_angle: -20
        end_angle: 80
        stroke_width: 15
        bar_color: "#EB3A7E"

        background: true
        bg_percentage: 1
        bg_color: "#85EB3A7E"
    }

    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -10
        text: "100"
        font.pixelSize: 95
        font.weight: Font.Bold
        color: "white"
        // lineHeight: 1.0
        // lineHeightMode: Text.FixedHeight
        // Layout.preferredHeight: contentHeight - 18
        font.hintingPreference: Font.PreferFullHinting
    }
    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 50
        text: "km/h"
        font.pixelSize: 20
        font.weight: 700
        color: "#66FFFFFF"
        lineHeight: 0
    }
}