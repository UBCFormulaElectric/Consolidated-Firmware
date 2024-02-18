import QtQuick
import QtQuick.Layouts

import constants
import switcher
import pages
import dimswitches

Window {
    minimumWidth: Constants.SCREEN_WIDTH
    minimumHeight: Constants.SCREEN_HEIGHT
    visible: true
    title: Constants.WINDOW_TITLE
    color: "#5BC236"

    Rectangle {
        width: Constants.SCREEN_WIDTH
        height: Constants.SCREEN_HEIGHT
        anchors.centerIn: parent

        // todo come up with a more professional solution
        Rectangle {
            color: "#141414"
            anchors.fill: parent
            id: mainStack
            property int currentIndex: 1
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
        Switcher { }
    }

    // DimSwitches {
    //     onButtonPressed: {
    //         console.log("happy time")
    //     }
    // }

    function handleOnButtonPressed() {
        console.log("happy time")
    }
    Connections {
        target: DimSwitchEmitter
        function onStgButtonPressed() {
            handleOnButtonPressed()
        }
    }
}