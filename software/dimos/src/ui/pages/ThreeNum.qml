import QtQuick
import canqml

Item {
    anchors.fill: parent

    Column {
        anchors.centerIn: parent
        Text {
            text: "SOC: " + Math.round(CanQML.BMS_Soc, 2) + "%"
            color: "white"
            font.pixelSize: 40
        }

        Text {
            text: "MinCellV: " + Math.round(CanQML.BMS_MinCellVoltage, 2) + "V"
            color: "white"
            font.pixelSize: 40
        }

        Text {
            text: "MaxCellT: " + Math.round(CanQML.BMS_MaxCellTemperature, 2) + "C"
            color: "white"
            font.pixelSize: 40
        }
    }
}