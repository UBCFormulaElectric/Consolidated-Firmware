import QtQuick
import QtQuick.Layouts
import components

Item {
    anchors.fill: parent

    Item {
        id: enduranceSpeedometer
        width: 250
        height: 250

        readonly property int mainStroke: 6
        readonly property int bgStroke: 3

        CircleProgressBar {
            anchors.fill: parent
            anchors.bottomMargin: (enduranceSpeedometer.mainStroke - enduranceSpeedometer.bgStroke) / 2
            anchors.topMargin: (enduranceSpeedometer.mainStroke - enduranceSpeedometer.bgStroke) / 2
            anchors.leftMargin: (enduranceSpeedometer.mainStroke - enduranceSpeedometer.bgStroke) / 2
            anchors.rightMargin: (enduranceSpeedometer.mainStroke - enduranceSpeedometer.bgStroke) / 2

            round_tip: true
            percentage: 0.5 + 0.5 * (1) // TODO bind this to DCMPowerLimitAtFullThrottle, AvailablePower?
            start_angle: 90
            end_angle: -270

            stroke_width: enduranceSpeedometer.bgStroke
            bar_color: "grey"
            background: false
        }
        CircleProgressBar {
            anchors.fill: parent

            round_tip: true
            percentage: -0.5 // TODO bind this to TractiveSystemPower
            start_angle: -90
            end_angle: -270

            stroke_width: enduranceSpeedometer.mainStroke
            bar_color: "white"
            background: false
        }
        ColumnLayout {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            Text {
                text: "100"
                font.pixelSize: 80
                font.bold: true
                color: "white"
                Layout.alignment: Qt.AlignHCenter
            }
            Text {
                text: "km/h"
                font.pixelSize: 20
                color: "grey"
                Layout.alignment: Qt.AlignHCenter
            }
        }
    }
}