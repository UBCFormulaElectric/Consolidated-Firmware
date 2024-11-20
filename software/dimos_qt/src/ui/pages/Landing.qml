import QtQuick
import dimswitches

FocusScope {
    anchors.fill: parent
    Image {
        anchors.centerIn: parent
        source: "qrc:/fe_logo.svg"
    }

    DimSwitchNav{
        out: switcher
    }
}