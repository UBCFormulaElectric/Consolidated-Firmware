import QtQuick

Item {
    id: root

    property color outerColor: "#595959"
    property color innerColor: "#53ff44"

    property int radius: 50
    property int strokeWeight: 10

    width: radius*2
    height: radius*2

    Canvas {

    }
}