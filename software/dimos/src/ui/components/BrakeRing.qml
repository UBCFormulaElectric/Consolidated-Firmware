import QtQuick
import components
import canqml
import constants
import Qt5Compat.GraphicalEffects

Item {
    // Bind brake CAN stuff here
    // Debugging with key later
    property int frontBrakePressure: CanQML.FSM_FrontBrakePressure
    property int rearBrakePressure: CanQML.FSM_RearBrakePressure
    property real pressurePercentage: (frontBrakePressure + rearBrakePressure)/Constants.maxBrakePressure*2

    // default values
    property int size: 200
    property real colorFraction: 0

    // im sorry crodie
    property int red: 251
    property int green: 220
    property int blue: 54

    // im cheesin fam
    property real redDif: -197
    property real greenDif: 31
    property real blueDif: 43

    Rectangle {
        anchors.fill: parent
        id: ring
        height: size
        width: size
        radius: size/2
        border.color: Qt.rgba(red + redDif*colorFraction,
            green + greenDif*colorFraction, blue + blueDif*colorFraction, 1)
        color: "transparent"
        border.width: 10
    }

    Glow {
        anchors.fill: ring
        radius: 10
        samples: 50 * pressurePercentage
        color: Qt.rgba(red + redDif*colorFraction,
            green + greenDif*colorFraction, blue + blueDif*colorFraction, 1)
        source: ring
    }
}