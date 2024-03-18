import QtQuick
import QtQuick.Layouts
import components

Item {
    GridLayout {
        anchors.fill: parent
        columns: 4
        rows: 3
        uniformCellHeights: true
        uniformCellWidths: true
        id: grid

        columnSpacing: 30
        rowSpacing: 10
        anchors.leftMargin: 90
        anchors.rightMargin: 90
        anchors.topMargin: 80
        anchors.bottomMargin: 80

        Rectangle {
            color: "red"
            Layout.columnSpan: 2
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
        Item {
            Layout.rowSpan: 2
            Layout.columnSpan: 2
            Layout.fillHeight: true
            Layout.fillWidth: true

            ShutdownLoop {
                id: shutdownLoop
                anchors.fill: parent
                visible: true
            }
        }

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Squircle {
                height: 190; width: 190
                radius: 25
                bg_color: "#fff"
                smoothness: 0.6
                Image {
                    source: "qrc:/fe_logo.svg"
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    width: parent.width * 0.8
                    fillMode: Image.PreserveAspectFit
                }
            }
            Text {
                text: "UBC Formula Electric"
                color: "white"
                font.pixelSize: 15
                font.bold: true
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
            }
        }
        Item {
            Layout.preferredWidth: 190
            Layout.preferredHeight: 210

            Squircle {
                height: 190; width: 190
                radius: 25
                bg_color: "#002D67"
                smoothness: 0.6

                id: batteryChargeComponent
                readonly property int chargeHeight: 43
                readonly property int progressBarWidth: 17

                CircleProgressBar {
                    width: 120
                    height: 120
                    percentage: 0.79
                    stroke_width: batteryChargeComponent.progressBarWidth
                    start_angle: 90
                    end_angle: -270
                    bar_color: "#36FB61"
                    bg_color: "#40B8B8B8" // NOTE THAT THIS IS "AARRGGBB" FORMAT
                    round_tip: true
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter

                    Image {
                        source: "qrc:/battery.svg"
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        height: 40
                        fillMode: Image.PreserveAspectFit
                    }
                    Image {
                        source: "qrc:/charge.svg"
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.top: parent.top

                        height: batteryChargeComponent.chargeHeight
                        fillMode: Image.PreserveAspectFit
                        anchors.topMargin: batteryChargeComponent.progressBarWidth / 2 - batteryChargeComponent.chargeHeight / 2 - 2
                    }
                }
            }

            Text {
                text: "LV SOC"
                color: "white"
                font.pixelSize: 15
                font.bold: true
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
            }
        }
    }
}