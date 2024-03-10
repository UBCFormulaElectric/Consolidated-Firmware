import QtQuick

import components
import constants

Item {
    anchors.fill: parent
    default property alias content: childContainer.children
    readonly property real child_ratio: 0.4

    Item {
        id: notifContainer
        height: parent.height
        width: parent.width * (1-child_ratio)

        Rectangle {
            id: notifRectangle
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 15
            height: parent.height * 0.9
            width: parent.width;
            color: "#94979a"
            opacity: 0.3
            radius: 15
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
                property int iconSize: 80
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width*0.95
                height: 100
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
                        anchors.topMargin: 10
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
                        anchors.topMargin: 20
                        font.family: "SF Pro"
                        font.bold: true
                        font.pointSize: 24
                        text: name
                        color: "#000000"
                    }
                    Text {
                        anchors.left: icon.right
                        anchors.leftMargin: 10
                        anchors.top: parent.top
                        anchors.topMargin: 50
                        anchors.horizontalCenter: parent.horizontalCenter
                        font.family: "SF Pro"
                        font.pointSize: 16
                        text: description
                        color: "#000000"
                    }
                }
            }
        }

        Text{
            text: "COME HOME THE KIDS MISS YOU"
            anchors.horizontalCenter: parent.horizontalCenter
            font.family: "SF Pro"
            font.bold: true
            font.pointSize: 24
            color: "#ffffff"
        }

        ListView {
            anchors.top: notifRectangle.top
            anchors.topMargin: 15
            anchors.fill: notifRectangle
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