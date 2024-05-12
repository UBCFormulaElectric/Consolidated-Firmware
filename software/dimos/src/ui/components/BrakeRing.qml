import QtQuick
import components
import canqml
import constants
import Qt5Compat.GraphicalEffects

Item {
    anchors.fill: parent
    // Bind brake CAN stuff here
    // Debugging with key later
    property int frontBrakePressure: CanQML.FSM_FrontBrakePressure
    property int rearBrakePressure: CanQML.FSM_RearBrakePressure
    property real pressurePercentage: (frontBrakePressure + rearBrakePressure)/Constants.maxBrakePressure*2

    // default values
    property int size: 200
    property real colorFraction: 0
    property int borderWidth: 5

    // im sorry crodie
    property int red: 54
    property int green: 251
    property int blue: 97

    // im cheesin fam
    property real redDif: 197
    property real greenDif: -31
    property real blueDif: -43

    Rectangle {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenterOffset: 10
        id: ring
        height: size
        width: size
        radius: size/2
        border.color: Qt.rgba((red + redDif*colorFraction)/255,
            (green + greenDif*colorFraction)/255, (blue + blueDif*colorFraction)/255, 1)
        color: "transparent"
        border.width: borderWidth
    }

    Glow {
        anchors.fill: ring
        radius: 10
        samples: 10 * pressurePercentage
        color: Qt.rgba((red + redDif*colorFraction)/255,
            (green + greenDif*colorFraction)/255, (blue + blueDif*colorFraction)/255, 1)
        source: ring
    }

    // debug
    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: ring.bottom
        anchors.topMargin: 30
        font.family: "Roboto"
        font.pointSize: 16
        text: green + greenDif*colorFraction
        color: "#ffffff"
    }

}