import QtQuick

Item {
    anchors.fill: parent
    default property alias content: childContainer.children

    readonly property real child_ratio: 0.4
    Item {
        id: notifContainer
        height: parent.height
        width: parent.width * (1-child_ratio)
    }
    Item {
        id: childContainer
        height: parent.height
        width: parent.width * child_ratio
        anchors.right: parent.right
    }
}