import QtQuick
import QtQuick.Layouts
import components

Item {
    // Rectangle {
    //     readonly property int widthMargin: 90
    //     readonly property int heightMargin: 80
    //     width: parent.width - 90*2; height: parent.height - 80*2
    //     x: 90; y: 80
    // }
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
        Rectangle {
            color: "red"
            Layout.rowSpan: 2
            Layout.columnSpan: 2
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
        Rectangle {
            color: "red"
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
        Item {
            Layout.preferredWidth: 190
            Layout.preferredHeight: 210

            ColumnLayout {
                Squircle {
                    Layout.preferredWidth: 190
                    Layout.preferredHeight: 190
                    radius: 25
                    color: "#002D67"
                    CircleProgressBar {
                        width: 120
                        height: 120
                        percentage: 0.79
                        stroke_width: 20
                        start_angle: 90
                        end_angle: -270
                        bar_color: "#36FB61"
                        bg_color: "#40B8B8B8" // NOTE THAT THIS IS "AARRGGBB" FORMAT
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
                Text {
                    text: "LV SOC"
                    color: "white"
                    font.pixelSize: 15
                    font.bold: true
                    Layout.alignment: Qt.AlignHCenter
                }
            }
        }
    }
}