import QtQuick
import Qt5Compat.GraphicalEffects
import canqml
import components
import constants

Item {
    anchors.fill: parent
    default property alias content: childContainer.children
    readonly property real child_ratio: 0.45

    // ListModel {
    //     id: errorModelExample
    //
    //     ListElement {
    //         name: "huh"
    //         description: "i dont need no molly to feel savage, when im on that molly i feel savage"
    //     }
    //     ListElement {
    //         name: "red"
    //         description: "all white gucci suit im feeling righteous, i know that the truth is hard to digest"
    //     }
    //     ListElement {
    //         name: "in"
    //         description: "so i will sing, my demons ten feet under me, inhale exhale but i cant breathe"
    //     }
    //     ListElement {
    //         name: "work"
    //         description: "sometimes, sometimes, someimtes"
    //     }
    // }

    // Page specifics
    Item {
        id: childContainer
        height: parent.height
        width: parent.width * child_ratio
        anchors.right: parent.right
    }

    Item {
        id: notifParent
        height: parent.height
        width: parent.width * (1 - child_ratio)
        Image {
            id: logoImage
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            source: "qrc:/fe_logo.svg"
        }
    }

    Component {
        id: errorDelegate
        Item {
            required property var modelData

            opacity: PathView.isCurrentItem ? 1 : 0
            anchors.top: parent.top
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            width: 425

            Text {
                anchors.top: parent.top
                anchors.topMargin: 35
                anchors.left: parent.left
                anchors.leftMargin: 40
                font.family: "SF Pro"
                font.bold: true
                font.pointSize: 20
                text: modelData.name
                color: "#ffffff"
            }
            Text {
                anchors.top: parent.top
                anchors.topMargin: 60
                anchors.left: parent.left
                anchors.leftMargin: 40
                width: parent.width - 80
                font.family: "SF Pro"
                font.pointSize: 12
                wrapMode: Text.WordWrap
                text: modelData.description
                color: "#ffffff"
            }
        }
    }

    Item {
        id: notifPanel
        height: 140
        width: 425
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10

        visible: CanQML.faults.length != 0

        Squircle {
            anchors.fill: parent
            id: notifContainer
            width: parent.width
            height: parent.height
            radius: 20
            smoothness: 0.6
            bg_color: "#FF3A45"
        }

        PathView {
            id: carousel
            anchors.bottom: parent.bottom
            anchors.verticalCenter: parent.verticalCenter
            model: CanQML.faults
            delegate: errorDelegate
            path: Path {
                startX: 0
            }
        }

        Timer {
            running: CanQML.faults.length != 0
            interval: 5000
            repeat: true
            onTriggered: {
                carousel.incrementCurrentIndex()
                console.log(CanQML.faults.length)
            }
        }

        Row {
            anchors.top: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: -25
            spacing: 5

            Repeater {
                anchors.fill: parent
                model: CanQML.faults // this is indicator for faults

                Rectangle {
                    required property int index
                    id: indicator
                    height: 10
                    width: 10
                    radius: 5
                    color: "#ffffff"
                    opacity: (index === carousel.currentIndex) ? 1 : 0.3
                }
            }
        }
    }
}