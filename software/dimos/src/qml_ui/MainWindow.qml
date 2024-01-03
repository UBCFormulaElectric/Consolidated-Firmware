import QtQuick
import QtQuick.Layouts

import constants
import switcher
import pages

Window {
    width: Constants.SCREEN_WIDTH
    height: Constants.SCREEN_HEIGHT
    visible: true
    title: "Hello World"

    // todo come up with a more professional solution
    Rectangle {
        id: mainStack
        anchors.fill: parent
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