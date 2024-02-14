import QtQuick

import squircle
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
            anchors.centerIn: parent
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
                colorType: "#FF3B30"
            }
            ListElement {
                name: "WARNING EXAMPLE"
                colorType: "#F6AA06"
            }
            ListElement {
                name: "DECAYED EXAMPLE"
                colorType: "#8C8C8C"
            }
        }

        Component {
            id: errorDelegate
            Item {
                width: parent.width*0.9
                height: 100
                Rectangle {
                    anchors.fill: parent
                    id: errorContainer
                    width: parent.width
                    height: parent.height
                    radius: 10
                    color: colorType
                    Text {
                        anchors.centerIn: parent
                        text: name
                        color: "#000000"
                    }
                }
            }
        }

        ListView {

            anchors.fill: notifRectangle
            anchors.horizontalCenter: notifRectangle
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