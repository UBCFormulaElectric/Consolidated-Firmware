import QtQuick
import QtQuick.Layouts

import constants
import switcher
import pages

Window {
    width: Constants.SCREEN_WIDTH
    height: Constants.SCREEN_HEIGHT
    visible: true
    title: qsTr("Hello World")

    Switcher {}

    StackLayout {
        id: mainStack
        // anchors.fill: parent
        height: 100
        width: 100
        currentIndex: 0
        Landing {}
    }
}