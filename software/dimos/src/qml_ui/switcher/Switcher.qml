import QtQuick

import squircle
import constants

Item {
    anchors.fill: parent

    ListModel {
        id: appModel
        ListElement {
            icon: "qrc:/SwitcherEventIcons/Endurance.svg"
        }
        ListElement {
            icon: "qrc:/SwitcherEventIcons/Acceleration.svg"
        }
        ListElement {
            icon: "qrc:/SwitcherEventIcons/Skidpad.svg"
        }
        ListElement {
            icon: "qrc:/SwitcherEventIcons/Autocross.svg"
        }
        ListElement {
            icon: "qrc:/SwitcherEventIcons/Brake.svg"
        }
        ListElement {
            icon: "qrc:/SwitcherEventIcons/LV.svg"
        }
    }

    Component {
        id: appIcon
        Squircle {
            id: squircleAppIcon
            required property url icon

            radius: 40
            smoothness: 0.3
            width: 200
            height: 200
            color: "#013372"
            Image {
                source: squircleAppIcon.icon
                anchors.centerIn: parent
                sourceSize.width: parent.width
                sourceSize.height: parent.height
                width: parent.width * 0.7
                height: parent.height
                fillMode: Image.PreserveAspectFit
            }
        }
    }

    PathView {
        anchors.fill: parent
        model: appModel
        delegate: appIcon
        path: Path {
            id: appIconTravelPath
            readonly property int bottom_offset: 200

            startX: Constants.SCREEN_WIDTH/2; startY: Constants.SCREEN_HEIGHT - appIconTravelPath.bottom_offset
            PathArc {
                x: Constants.SCREEN_WIDTH; y: Constants.SCREEN_HEIGHT
                radiusX: 950; radiusY: 450
            }
            PathMove { x: 0; y: Constants.SCREEN_HEIGHT }
            PathArc {
                x: Constants.SCREEN_WIDTH/2; y: Constants.SCREEN_HEIGHT - appIconTravelPath.bottom_offset
                radiusX: 950; radiusY: 450
            }
        }

        focus: true
        Keys.onLeftPressed: decrementCurrentIndex()
        Keys.onRightPressed: incrementCurrentIndex()
    }

    Text {
        text: qsTr("Selected Event")
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        color: "white"
        font.pixelSize: 30
        font.weight: 600
    }
}