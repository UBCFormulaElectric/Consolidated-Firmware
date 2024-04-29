import QtQuick
import QtQuick.Layouts
import components

FocusScope {
    anchors.fill: parent

    Speedometer {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -40
        id: enduranceSpeedometer
    }

    ColumnLayout {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 1
            color: "#63676B"
        }
        GridLayout {
            implicitWidth: parent.width * 0.85
            Layout.alignment: Qt.AlignHCenter
            Layout.fillWidth: false
            columns: 2
            rows: 2
            columnSpacing: 30
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