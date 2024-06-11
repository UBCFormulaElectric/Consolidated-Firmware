import QtQuick
import canqml

Item {
    anchors.fill: parent

    readonly property int padRadius: 10

    property real lateralAcceleration: CanQML.VC_AccelerationLateral

    Rectangle {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        width: 200
        height: 200
        radius: 100

        color: "transparent"
        border.color: "#797979"
        border.width: 5
    }

    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        font.pointSize: 36
        text: lateralAcceleration + " m/s"
        color: "#ffffff"
    }

    // Text {
    //     anchors.horizontalCenter: parent.horizontalCenter
    //     anchors.verticalCenter: parent.verticalCenter
    //
    //     font.family: "Roboto"
    //     font.pointSize: 36
    //     text: "drop and give me fifty"
    //     color: "#ff0000"
    // }
}