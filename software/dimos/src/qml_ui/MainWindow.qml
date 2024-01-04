import QtQuick
import QtQuick.Layouts

import constants
import switcher
import pages

Window {
    minimumWidth: Constants.SCREEN_WIDTH
    minimumHeight: Constants.SCREEN_HEIGHT
    visible: true
    title: Constants.WINDOW_TITLE
    color: "transparent"

    // todo come up with a more professional solution
    Rectangle {
        id: mainStack
        width: Constants.SCREEN_WIDTH
        height: Constants.SCREEN_HEIGHT
        anchors.centerIn: parent
        property int currentIndex: 0
        color: "#141414"
        Landing {
            visible: mainStack.currentIndex === 0
        }
        RacingBase {
            visible: mainStack.currentIndex === 1
            Acceleration {}
        }
        RacingBase {
            visible: mainStack.currentIndex === 2
            Autocross {}
        }
        RacingBase {
            visible: mainStack.currentIndex === 3
            Braking {}
        }
        RacingBase {
            visible: mainStack.currentIndex === 4
            Endurance {}
        }
        RacingBase {
            visible: mainStack.currentIndex === 5
            Skidpad {}
        }
    }

    Switcher {}
}