import QtQuick
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

import constants
import switcher
import pages
import dimswitches
import canqml

Window {
    minimumWidth: Constants.SCREEN_WIDTH
    minimumHeight: Constants.SCREEN_HEIGHT
    visible: true
    title: Constants.WINDOW_TITLE
    color: "#5BC236"

    // True window
    Rectangle {
        width: Constants.SCREEN_WIDTH
        height: Constants.SCREEN_HEIGHT
        anchors.centerIn: parent

        // todo come up with a more professional solution
        Rectangle {
            color: "#141414"
            anchors.fill: parent
            id: mainStack
            property int currentIndex: -1
            Landing {
                id: landingPage
                visible: mainStack.currentIndex === -1
                focus: true
            }
            LowVoltage {
                id: lowVoltagePage
                visible: mainStack.currentIndex === 0
            }
            RacingBase {
                visible: [1, 2, 3, 4, 5].includes(mainStack.currentIndex)
                Acceleration {
                    id: accelerationPage
                    visible: mainStack.currentIndex === 1
                }
                Autocross {
                    id: autocrossPage
                    visible: mainStack.currentIndex === 2
                }
                Braking {
                    id: brakingPage
                    visible: mainStack.currentIndex === 3
                }
                Endurance {
                    id: endurancePage
                    visible: mainStack.currentIndex === 4
                }
                Skidpad {
                    id: skidpadPage
                    visible: mainStack.currentIndex === 5
                }
            }
            DebugSoftware {
                visible: mainStack.currentIndex === 6
                id: softwareDebugPage
            }
        }

        GaussianBlur {
            anchors.fill: mainStack
            source: mainStack
            radius: 32
            samples: 32
            visible: switcher.activeFocus
        }
        Switcher {
            id: switcher
        }
        Shutdown {
            visible: false
        }
        // Text {
        //     anchors.centerIn: parent
        //     text: CanQML.VC_Fault_DummyFault ? "Fault" : "No Fault"
        //     color: "white"
        // }
    }
}