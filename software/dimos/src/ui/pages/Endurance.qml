import QtQuick
import QtQuick.Layouts
import components

Item {
    anchors.fill: parent

    Item {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        id: enduranceSpeedometer
        width: 250
        height: 250

        readonly property int mainStroke: 6
        readonly property int bgStroke: 3

        CircleProgressBar {
            anchors.fill: parent
            round_tip: true
            start_angle: -90
            end_angle: -270
            bg_start_angle: 90
            bg_end_angle: -270

            percentage: -0.5 // TODO bind this to TractiveSystemPower
            bg_percentage: 0.5 + 0.5 * (0.5) // TODO bind this to DCMPowerLimitAtFullThrottle, AvailablePower?

            stroke_width: enduranceSpeedometer.mainStroke
            bar_color: "white"

            background: true
            bg_stroke_width: enduranceSpeedometer.bgStroke
            bg_color: "grey"
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

    Row {
        anchors.left: enduranceSpeedometer.left
        anchors.top: enduranceSpeedometer.top
        Rectangle {
            Image {
                source: "qrc:/battery.svg"
                width: 50
                fillMode: Image.PreserveAspectFit
                transform: Rotation {
                    angle: 90
                }
            }
        }
        Text {
            text: "100%"
            color: "white"
        }
        Text {
            text: "99km"
            color: "white"
        }
    }
}