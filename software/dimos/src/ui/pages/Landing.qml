import QtQuick
import JSONCANQML

Item {
    anchors.fill: parent
    Image {
        anchors.centerIn: parent
        source: "qrc:/fe_logo.svg"
    }

    Text {
        text: JSONCANQML.VC_Fault_DummyFault ? "Fault" : "No Fault"
        color: "white"
    }
}