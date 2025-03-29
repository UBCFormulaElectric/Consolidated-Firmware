import QtQuick
import canqml

Item {
    anchors.fill: parent

    Column {
        Text {
            text: Math.round(CanQML.BMS_Soc, 2) + "%"
            color: "white"
            font.pixelSize: 80
            font.bold: true
        }

        Text {
            text: Math.round(CanQML.BMS_MinCellVoltage, 2) + "V"
            color: "white"
            font.pixelSize: 80
            font.bold: true
        }

        Text {
            text: Math.round(CanQML.BMS_MaxCellTemperature, 2) + "C"
            color: "white"
            font.pixelSize: 80
            font.bold: true
        }

        Text {
            text: Math.round(CanQML.INVL_GateDriverBoardTemperature, 2) + "C"
            color: "white"
            font.pixelSize: 80
            font.bold: true
        }

        Text {
            text: Math.round(CanQML.INVR_MotorTemperature, 2) + "C"
            color: "white"
            font.pixelSize: 80
            font.bold: true
        }
    }
}