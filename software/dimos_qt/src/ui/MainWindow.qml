import QtQuick
import QtQuick.Layouts

import constants
import switcher
import pages
import dimswitches
import canqml

Window {
    minimumWidth: Constants.SCREEN_WIDTH
    minimumHeight: Constants.SCREEN_HEIGHT
    visible: true
    visibility: Constants.deploy ? Window.FullScreen : Window.AutomaticVisibility;
    title: Constants.WINDOW_TITLE
    color: "#5BC236"

    // True window
    Rectangle {
        width: Constants.SCREEN_WIDTH
        height: Constants.SCREEN_HEIGHT
        anchors.centerIn: parent
        rotation: Constants.deploy ? 180 : 0

        // todo come up with a more professional solution
        Rectangle {
            color: "#141414"
            anchors.fill: parent
            id: mainStack

            property int currentIndex: 0
            Landing {
                id: landingPage
                visible: mainStack.currentIndex === -1
                focus: true
            }
            LowVoltage {
                id: lowVoltagePage
                visible: mainStack.currentIndex === 0
            }
            // this is not great practice especially considering that it does not genearlized well
            // if the mainStack was a real component
            DrivingBase {
                visible: [1, 2, 3, 4, 6].includes(mainStack.currentIndex)
                Image {
                    function getPageIconURL(index: int): string {
                        switch (index) {
                            case 1:
                                return "qrc:/SwitcherEventIcons/Acceleration.svg"
                            case 2:
                                return "qrc:/SwitcherEventIcons/Brake.svg"
                            case 3:
                                return "qrc:/SwitcherEventIcons/Endurance.svg"
                            case 4:
                                return "qrc:/SwitcherEventIcons/Skidpad.svg"
                            case 6:
                                return "qrc:/SwitcherEventIcons/Endurance.svg"
                            default:
                                return ""
                        }
                    }

                    anchors.top: parent.top
                    anchors.right: parent.right
                    anchors.margins: 10
                    width: 50
                    height: 50
                    source: getPageIconURL(mainStack.currentIndex)
                }
                Acceleration {
                    id: accelerationPage
                    visible: mainStack.currentIndex === 1
                }
                Braking {
                    id: brakingPage
                    visible: mainStack.currentIndex === 2
                }
                Endurance {
                    id: endurancePage
                    visible: mainStack.currentIndex === 3
                }
                Skidpad {
                    id: skidpadPage
                    visible: mainStack.currentIndex === 4
                }
                ThreeNum {
                    id: threenum
                    visible: mainStack.currentIndex === 6
                }
            }
            DebugSoftware {
                visible: mainStack.currentIndex === 5
                id: softwareDebugPage
            }

            function refocusMainStack() {
                let nextPage;
                switch (mainStack.currentIndex) {
                    case 0:
                        nextPage = lowVoltagePage
                        break;
                    case 1:
                        nextPage = accelerationPage
                        break;
                    case 2:
                        nextPage = brakingPage
                        break;
                    case 3:
                        nextPage = endurancePage
                        break;
                    case 4:
                        nextPage = skidpadPage
                        break;
                    case 5:
                        nextPage = softwareDebugPage
                        break;
                    case 6:
                        nextPage = threenum;
                        break;
                    default:
                        nextPage = landingPage
                        break;
                }
                nextPage.forceActiveFocus();
                if (nextPage.onPageLoad !== undefined) {
                    nextPage.onPageLoad();
                }
            }

            Component.onCompleted: {
                refocusMainStack()
            }
        }

        Switcher {
            id: switcher
        }
        // Shutdown {
        //     id: shutdown_element
        //     property bool override: false
        //     focus: true
        //     opacity: CanQML.has_fault && (override === false) ? 1 : 0
        //     Behavior on opacity {
        //         NumberAnimation {
        //             duration: 1000
        //         }
        //     }
        //
        //     DimSwitches {
        //         onPushRot: {
        //             if (!shutdown_element.activeFocus) return;
        //             if (CanQML.has_fault) {
        //                 shutdown_element.override = true;
        //                 mainStack.refocusMainStack();
        //             }
        //         }
        //     }
        //     Connections {
        //         target: CanQML
        //
        //         function onFaultChanged() {
        //             if (CanQML.has_fault) { // fault turns on
        //                 shutdown_element.override = false;
        //                 shutdown_element.focus = true;
        //             }
        //         }
        //     }
        // }
    }
}