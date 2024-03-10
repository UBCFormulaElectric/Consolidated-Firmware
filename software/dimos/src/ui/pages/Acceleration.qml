import QtQuick
import QtQuick.Shapes
import components

Item {
    anchors.fill: parent
    property var speed: 100 // Change to canQML

    Text {
        id: speedString
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 200
        font.family: "Roboto"
        font.bold: true
        font.pointSize: 82
        text: speed
        color: "#ffffff"
    }

    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: speedString.bottom
        anchors.topMargin: -5
        font.family: "Roboto"
        font.pointSize: 16
        text: "km/h"
        color: "#868686"
    }

    CircleProgressBar {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 135
        width: 250
        height: 250
        percentage: 0.80
        stroke_width: 25
        start_angle: 225
        end_angle: -45
        round_tip: true
        bar_color: "#36FB61"
        bg_color: "#696969"
    }

    CircleProgressBar {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 135
        width: 320
        height: 280
        percentage: 0.50
        stroke_width: 15
        start_angle: 235
        end_angle: 170
        round_tip: true
        bar_color: "#FBDC36"
        bg_color: "#696969"
    }

    CircleProgressBar {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 135
        width: 320
        height: 280
        percentage: 0.50
        stroke_width: 15
        start_angle: -55
        end_angle: 10
        round_tip: true
        bar_color: "#FBDC36"
        bg_color: "#696969"
    }


    // Try to get the accerlation bit to show, try to display everything
    // Then add animations to racing base and accel
}