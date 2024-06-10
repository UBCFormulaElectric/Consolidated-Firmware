import QtQuick
import Qt5Compat.GraphicalEffects

import components
import constants

Item {
    anchors.fill: parent
    default property alias content: childContainer.children
    readonly property real child_ratio: 0.45

    // Page specifics
    Item {
        id: childContainer
        height: parent.height
        width: parent.width * child_ratio
        anchors.right: parent.right
    }

    Item {
        id: notifParent
        height: parent.height
        width: parent.width * (1 - child_ratio)
        Image {
            id: logoImage
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            source: "qrc:/fe_logo.svg"
        }
    }
}