import QtQuick
import QtQuick.Layouts
import components

Item {
    anchors.fill: parent
    CircleProgressBar {
        id: circleProgressBar
        width: 250
        height: 250

        percentage: -0.5
        stroke_width: 3
        bar_stroke_width: 6
        start_angle: -90
        end_angle: -270
        round_tip: true
        bar_color: "white"
        bg_color: "grey"

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