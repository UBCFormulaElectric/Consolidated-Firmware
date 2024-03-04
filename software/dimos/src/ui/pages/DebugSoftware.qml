import QtQuick
import QtQuick.Layouts

import dimswitches

Item {
    anchors.fill: parent

    DimSwitches {
        id: dimSwitches
    }
    Rectangle {
        x: 220; y: 20
        radius: 100
        height: 30; width: 30
        color: dimSwitches.isOutButtonPressed ? "red": "gray"
    }
    Rectangle {
        x: 90; y: 280
        radius: 100
        height: 30; width: 30
        color: dimSwitches.isSettingsButtonPressed ? "red": "gray"
    }
    Rectangle {
        x: 220; y: 280
        radius: 100
        height: 30; width: 30
        color: dimSwitches.isErrButtonPressed ? "red": "gray"
    }
    Rectangle {
        x: 10; y: 180
        radius: 100
        height: 30; width: 30
        color: dimSwitches.isF1ButtonPressed ? "red": "gray"
    }
    Rectangle {
        x: 290; y: 180
        radius: 100
        height: 30; width: 30
        color: dimSwitches.isF2ButtonPressed ? "red": "gray"
    }
    Rectangle {
        x: 140; y: 130
        radius: 100
        height: 60; width: 60
        color: dimSwitches.isPushRotButtonPressed ? "red": "gray"
    }
}