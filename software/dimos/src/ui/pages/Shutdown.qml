import QtQuick
import QtQuick.Layouts

Item {
    anchors.fill: parent
    Rectangle {
        anchors.fill: parent
        color: "black"
        opacity: 0.8
    }
    ColumnLayout {
        anchors.centerIn: parent
        Text {
            text: "Shutdown"
            color: "white"
            font {
                pixelSize: 80
                bold: true
            }
            Layout.alignment: Qt.AlignHCenter
        }

        Text {
            text: "Error 1"
            color: "white"
            font {
                pixelSize: 40
            }
            Layout.alignment: Qt.AlignHCenter
        }
        Text {
            text: "Error 2"
            color: "white"
            font {
                pixelSize: 40
            }
            Layout.alignment: Qt.AlignHCenter
        }
        Text {
            text: "Error 3"
            color: "white"
            font {
                pixelSize: 40
            }
            Layout.alignment: Qt.AlignHCenter
        }

        Text {
            text: "Press OK to Continue"
            color: "white"
            font {
                pixelSize: 20
            }
            Layout.topMargin: 30
            Layout.alignment: Qt.AlignHCenter
        }
    }
}