import QtQuick
import Qt5Compat.GraphicalEffects

import components
import constants
import dimswitches

FocusScope {
    anchors.fill: parent
    readonly property int appIconSize: 225
    focus: true

    // todo improved model
    ListModel {
        id: appModel
        ListElement {
            icon: "qrc:/SwitcherEventIcons/LV.svg"
            name: "LV"
        }
        ListElement {
            icon: "qrc:/SwitcherEventIcons/Acceleration.svg"
            name: "Acceleration"
        }
        ListElement {
            icon: "qrc:/SwitcherEventIcons/Brake.svg"
            name: "Brake"
        }
        ListElement {
            icon: "qrc:/SwitcherEventIcons/Endurance.svg"
            name: "Endurance"
        }
        ListElement {
            icon: "qrc:/SwitcherEventIcons/Skidpad.svg"
            name: "Skidpad"
        }
        ListElement {
            icon: "qrc:/SwitcherEventIcons/SoftwareDebug.svg"
            name: "Software Debug"
        }
        ListElement {
            icon: "qrc:/SwitcherEventIcons/Endurance.svg"
            name: "Endurance ThreeVal"
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
            bg_color: "#013372"
            Image {
                source: squircleAppIcon.icon
                anchors.centerIn: parent
                sourceSize.width: parent.width
                sourceSize.height: parent.height
                width: parent.width * 0.6
                height: parent.height * 0.6
                fillMode: Image.PreserveAspectFit
            }
        }
    }

    GaussianBlur {
        anchors.fill: parent
        source: mainStack
        radius: switcher.activeFocus ? 32 : 0
        samples: 32
    }

    // todo custom animations
    PathView {
        anchors.fill: parent
        visible: parent.activeFocus
        model: appModel
        delegate: appIcon
        id: appIconPathView
        path: Path {
            id: appIconTravelPath
            // IMPORTANT SETTINGS
            readonly property int x_rad: 955
            readonly property int y_rad: 955
            readonly property int vertial_offset: 50
            readonly property real end_angles: 35
            readonly property real end_scale: 0.85

            readonly property int center_x: Constants.SCREEN_WIDTH / 2
            readonly property int center_y: Constants.SCREEN_HEIGHT + y_rad - vertial_offset - switcher.appIconSize / 2
            startX: 0; startY: Constants.SCREEN_HEIGHT + appIconTravelPath.vertial_offset
            PathAttribute {
                name: "iconScale"; value: appIconTravelPath.end_scale
            }

            PathAngleArc {
                centerX: appIconTravelPath.center_x; centerY: appIconTravelPath.center_y
                radiusX: appIconTravelPath.x_rad; radiusY: appIconTravelPath.y_rad
                startAngle: 270 - appIconTravelPath.end_angles; sweepAngle: appIconTravelPath.end_angles
            }
            PathAttribute {
                name: "iconScale"; value: 1
            }

            PathAngleArc {
                centerX: appIconTravelPath.center_x; centerY: appIconTravelPath.center_y
                radiusX: appIconTravelPath.x_rad; radiusY: appIconTravelPath.y_rad
                startAngle: 270; sweepAngle: appIconTravelPath.end_angles
            }
            PathAttribute {
                name: "iconScale"; value: appIconTravelPath.end_scale
            }
        }

        preferredHighlightBegin: 0.5
        preferredHighlightEnd: 0.5
        highlightRangeMode: PathView.StrictlyEnforceRange
        clip: true
        pathItemCount: 5

        Component.onCompleted: {
            currentIndex = mainStack.currentIndex
        }

        DimSwitches {
            onLeftRot: {
                if (!switcher.activeFocus) return;
                // noinspection JSUnresolvedReference
                appIconPathView.decrementCurrentIndex()
            }
            onRightRot: {
                if (!switcher.activeFocus) return;
                // noinspection JSUnresolvedReference
                appIconPathView.incrementCurrentIndex()
            }
            onPushRot: {
                if (!switcher.activeFocus) return;
                mainStack.currentIndex = appIconPathView.currentIndex
                console.log("Navigating to page with index: ", appIconPathView.currentIndex)
                mainStack.refocusMainStack()
            }
            onOutButtonPressed: {
                if (!switcher.activeFocus) return;
                mainStack.refocusMainStack()
            }
        }
    }

    Text {
        text: appModel.get(appIconPathView.currentIndex).name
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 7
        anchors.horizontalCenter: parent.horizontalCenter
        opacity: parent.activeFocus ? 1 : 0
        Behavior on opacity {
            NumberAnimation {
                duration: 100
                easing.type: Easing.Linear
            }
        }
        color: "white"
        font.pixelSize: 24
        font.weight: 600
    }
}
