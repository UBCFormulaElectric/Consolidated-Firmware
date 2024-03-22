import QtQuick
import Qt5Compat.GraphicalEffects

import components
import constants

Item {
    anchors.fill: parent
    default property alias content: childContainer.children
    readonly property real child_ratio: 0.4

    Item {
        id: notifParent
        height: parent.height
        width: parent.width * (1-child_ratio)

        Squircle {
            id: notifContainer
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 15
            height: parent.height * 0.9
            width: parent.width;
            bg_color: "#94979a"
            opacity: 0.3
            radius: 15
            smoothness: 0.6
            Image {
                id: logoImage
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                source: "qrc:/fe_logo.svg"
            }

            GaussianBlur {
                anchors.fill: logoImage
                source: logoImage
                radius: 60
                samples: 16
            }

        }

        ListModel {
            id: errorModel

            // Add image and subtext later
            ListElement {
                name: "ERROR EXAMPLE"
                description: "This is an error"
                colorType: "#FF3B30"
            }
            ListElement {
                name: "WARNING EXAMPLE"
                description: "This is a warning"
                colorType: "#F6AA06"
            }
            ListElement {
                name: "DECAYED EXAMPLE"
                description: "This notification has decayed"
                colorType: "#8C8C8C"
            }
        }

        Component {
            id: errorDelegate
            Item {
                property int iconSize: notifContainer.height*0.15 * 0.8
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width*0.95
                height: notifContainer.height*0.15
                Rectangle {
                    anchors.fill: parent
                    id: errorContainer
                    width: parent.width
                    height: parent.height
                    radius: 10
                    color: colorType
                    Rectangle { // Todo: add icons
                        id: icon
                        anchors.top: parent.top
                        anchors.topMargin: parent.height*0.1
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        width: iconSize
                        height: iconSize
                        radius: 15
                        color: "#FFFFFF"
                        opacity: 50
                    }
                    Text {
                        anchors.left: icon.right
                        anchors.leftMargin: 10
                        anchors.top: parent.top
                        anchors.topMargin: parent.height*0.2
                        font.family: "SF Pro"
                        font.bold: true
                        font.pointSize: 20
                        text: name
                        color: "#000000"
                    }
                    Text {
                        anchors.left: icon.right
                        anchors.leftMargin: 10
                        anchors.top: parent.top
                        anchors.topMargin: parent.height*0.5
                        anchors.horizontalCenter: parent.horizontalCenter
                        font.family: "SF Pro"
                        font.pointSize: 14
                        text: description
                        color: "#000000"
                    }
                }
            }
        }

        ListView {
            anchors.top: notifContainer.top
            anchors.topMargin: 15
            anchors.fill: notifContainer
            model: errorModel
            delegate: errorDelegate
            clip: true
            spacing: 10
        }
    }

    // Page specifics
    Item {
        id: childContainer
        height: parent.height
        width: parent.width * child_ratio
        anchors.right: parent.right
    }
}