import QtQuick
import QtQuick.Layouts
import components

Item {
    anchors.fill: parent

    Item {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        id: enduranceSpeedometer
        width: 300
        height: 300

        readonly property real instantaneous_power_percentage: 0.5 // TODO bind this to TractiveSystemPower
        readonly property real avaiable_power_percentage: 0.75 // TODO bind this to DCMPowerLimitAtFullThrottle, AvailablePower

        readonly property real hv_battery_soc: 0.5 // TODO bind these to
        readonly property real max_cell_temp: 0.5 // TODO bind these to


        CircleProgressBar {
            anchors.fill: parent
            anchors.margins: 26
            round_tip: true

            percentage: enduranceSpeedometer.instantaneous_power_percentage
            start_angle: -90
            end_angle: -270
            stroke_width: 6
            bar_color: enduranceSpeedometer.instantaneous_power_percentage >= 0 ? "white" : "#0CFF41"

            bg_percentage: 0.5 + 0.5 * enduranceSpeedometer.avaiable_power_percentage
            bg_start_angle: 90
            bg_end_angle: -270
            background: true
            bg_stroke_width: 3
            bg_color: "grey"
        }
        CircleProgressBar {
            anchors.fill: parent
            round_tip: true

            percentage: enduranceSpeedometer.hv_battery_soc
            start_angle: -140
            end_angle: -40
            stroke_width: 10
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
            stroke_width: 10
            bar_color: "#EB3A7E"

            background: true
            bg_percentage: 1
            bg_color: "#85EB3A7E"
        }

        // center text
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

    ColumnLayout {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width * 0.9
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 1
            color: "#63676B"
        }
        GridLayout {
            width: parent.width
            columns: 2
            rows: 2
            columnSpacing: 20
            Repeater {
                model: [{
                    key: "VAL 1",
                    value: "100V"
                }, {
                    key: "VAL 2",
                    value: "100V"
                }, {
                    key: "VAL 3",
                    value: "100V"
                }, {
                    key: "VAL 4",
                    value: "100V"
                }]
                Item {
                    Layout.fillWidth: true
                    Layout.rowSpan: 1
                    Layout.columnSpan: 1
                    Layout.preferredHeight: 34
                    required property var modelData

                    Text {
                        text: modelData.key
                        color: "darkgrey"
                        font.pixelSize: 16
                        font.weight: Font.Bold
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                    }
                    Text {
                        text: modelData.value
                        color: "white"
                        font.pixelSize: 24
                        font.weight: Font.ExtraBold
                        anchors.bottom: parent.bottom
                        anchors.right: parent.right
                    }
                }
            }
        }
    }
}