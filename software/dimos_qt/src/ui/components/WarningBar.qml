import QtQuick
import components
import canqml
import constants
import Qt5Compat.GraphicalEffects

Item {
    anchors.fill: parent

    Component {
        id: warningDelegate

        required property var modelData
        
        // image
    }


    PathView {
        id: taskbar
        anchors.fill: parent
        model: CanQML.warnings
        delegate: warningDelegate
        path: Path {
            startX: 0
        }
    }
}