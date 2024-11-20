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
    property real pressurePercentage: (frontBrakePressure + rearBrakePressure) / Constants.maxBrakePressure * 2

    // default values
    required property int size
    required property int borderWidth
    required property real colorFraction

    readonly property var hsv1: [0.14, 0, 0.17]
    readonly property var hsv2: [0.14, 0.96, 0.60]
    readonly property var hsv3: [0.37, 0.96, 0.60]

    function lerp(a, b, t) {
        return a + (b - a) * t;
    }

    function lerpAngle(a, b, t) {
        let diff = b - a;
        diff = (diff + 180) % 360 - 180;  // Wrap the difference to [-180, 180]
        return a + diff * t;
    }

    function getColor(t) {
        let h, s, v;
        if (t < 0.5) {
            t *= 2;
            h = lerpAngle(hsv1[0], hsv2[0], t);
            s = lerp(hsv1[1], hsv2[1], t);
            v = lerp(hsv1[2], hsv2[2], t);
        } else {
            t = (t - 0.5) * 2;
            h = lerpAngle(hsv2[0], hsv3[0], t);
            s = lerp(hsv2[1], hsv3[1], t);
            v = lerp(hsv2[2], hsv3[2], t);
        }
        return Qt.hsla((h + 360) % 360, s, v, 1);
    }

    Rectangle {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenterOffset: 10
        id: ring
        height: size
        width: size
        radius: size / 2
        border.color: parent.getColor(colorFraction)
        color: "transparent"
        border.width: borderWidth
    }

    Glow {
        anchors.fill: ring
        radius: 10
        samples: 10 * pressurePercentage
        color: parent.getColor(colorFraction)
        source: ring
    }
}