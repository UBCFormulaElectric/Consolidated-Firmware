// noinspection PointlessArithmeticExpressionJS

import QtQuick
import QtQuick.Effects

import squircle
import constants

Item {
    id: switcher
    anchors.fill: parent
    readonly property int appIconSize: 225

    ListModel {
        id: appModel
        ListElement {
            icon: "qrc:/SwitcherEventIcons/Endurance.svg"
        }
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
            radius: 50
            smoothness: 0.6
            width: switcher.appIconSize
            height: switcher.appIconSize
            scale: PathView.iconScale
            color: "#013372"
            Image {
                source: squircleAppIcon.icon
                anchors.centerIn: parent
                sourceSize.width: parent.width
                sourceSize.height: parent.height
                width: parent.width * 0.7
                height: parent.height * 0.7
                fillMode: Image.PreserveAspectFit
            }
        }
    }


    MultiEffect {
        source: mainStack
        anchors.fill: parent
        blurEnabled: true
        blurMax: 64
        blur: 0.8
    }

    PathView {
        anchors.fill: parent
        model: appModel
        delegate: appIcon
        path: Path {
            id: appIconTravelPath
            // IMPORTANT SETTINGS
            readonly property int x_rad: 955
            readonly property int y_rad: 955
            readonly property int vertial_offset: 50
            readonly property real end_angles: 35
            readonly property real end_scale: 0.85

            readonly property int center_x: Constants.SCREEN_WIDTH / 2
            readonly property int center_y: Constants.SCREEN_HEIGHT + y_rad - vertial_offset - switcher.appIconSize/2
            startX: 0; startY: Constants.SCREEN_HEIGHT + appIconTravelPath.vertial_offset
            PathAttribute { name: "iconScale"; value: appIconTravelPath.end_scale }

            PathAngleArc {
                centerX: appIconTravelPath.center_x; centerY: appIconTravelPath.center_y
                radiusX: appIconTravelPath.x_rad; radiusY: appIconTravelPath.y_rad
                startAngle: 270 - appIconTravelPath.end_angles; sweepAngle: appIconTravelPath.end_angles
            }
            PathAttribute { name: "iconScale"; value: 1 }

            PathAngleArc {
                centerX: appIconTravelPath.center_x; centerY: appIconTravelPath.center_y
                radiusX: appIconTravelPath.x_rad; radiusY: appIconTravelPath.y_rad
                startAngle: 270; sweepAngle: appIconTravelPath.end_angles
            }
            PathAttribute { name: "iconScale"; value: appIconTravelPath.end_scale }
        }

        preferredHighlightBegin: 0.5
        preferredHighlightEnd: 0.5
        highlightRangeMode: PathView.StrictlyEnforceRange
        clip: true
        pathItemCount: 5
        focus: true
        Keys.onLeftPressed: decrementCurrentIndex()
        Keys.onRightPressed: incrementCurrentIndex()
    }

    Text {
        text: qsTr("Selected Event")
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 7
        anchors.horizontalCenter: parent.horizontalCenter
        color: "white"
        font.pixelSize: 24
        font.weight: 600
    }
}