import QtQuick
import canqml

Item {
    anchors.fill: parent

    readonly property int padRadius: 10

    property real lateralAcceleration: 0 // TODO GPS is not working CanQML.VC_AccelerationLateral


    Repeater {
        model: [4, 2]

        Rectangle {
            required property int modelData
            id: ring
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter

            width: 100 * modelData
            height: 100 * modelData
            radius: 50 * modelData

            color: "transparent"
            border.color: "#ffffff"
            border.width: 5
            opacity: 0.4
        }
    }

    Rectangle {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -lateralAcceleration * 5

        width: 10
        height: 10
        radius: 5

        color: "#36FB61"
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