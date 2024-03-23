import QtQuick
import QtQuick.Layouts
import components

Item {
    property bool first_visit_complete: false
    onFocusChanged: {
        if (focus) {
            first_visit_complete = true;
        }
    }
    Keys.onSpacePressed: {
        first_visit_complete = false;
    }
    GridLayout {
        anchors.fill: parent
        columns: 4
        rows: 3
        id: grid

        columnSpacing: 30
        rowSpacing: 10
        anchors.leftMargin: 90
        anchors.rightMargin: 90
        anchors.topMargin: 80
        anchors.bottomMargin: 80

        Item {
            Layout.preferredWidth: 190 * 2 + 30
            Layout.preferredHeight: 210
            Layout.columnSpan: 2

            id: driverProfileComponent
            Rectangle {
                anchors.fill: parent
                color: "red"
            }
        }

        Item {
            id: shutdownLoopComponent
            Layout.preferredWidth: 190 * 2 + 30
            Layout.preferredHeight: 210 * 2 + 10
            Layout.rowSpan: 2
            Layout.columnSpan: 2

            ShutdownLoop {
                id: shutdownLoop
                anchors.fill: parent
                visible: true
                percentage: first_visit_complete ? 0.803 : 0 // todo set to correct percentage

                Behavior on percentage {
                    NumberAnimation {
                        duration: 2500
                        easing.type: Easing.InOutSine
                    }
                }
            }
        }

        Item {
            Layout.preferredWidth: 190
            Layout.preferredHeight: 210
            id: logoComponent

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
            id: lvSOCComponent
            readonly property int chargeHeight: 43
            readonly property int progressBarWidth: 17

            Squircle {
                height: 190; width: 190
                radius: 25
                bg_color: "#002D67"
                smoothness: 0.6

                CircleProgressBar {
                    width: 120
                    height: 120
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter

                    round_tip: true

                    background: true
                    bar_color: "#36FB61"
                    stroke_width: lvSOCComponent.progressBarWidth
                    bg_color: "#40B8B8B8" // NOTE THAT THIS IS "AARRGGBB" FORMAT
                    bg_percentage: 1

                    percentage: 0.79
                    start_angle: 90
                    end_angle: -270
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

                        height: lvSOCComponent.chargeHeight
                        fillMode: Image.PreserveAspectFit
                        anchors.topMargin: lvSOCComponent.progressBarWidth / 2 - lvSOCComponent.chargeHeight / 2 - 2
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