import QtQuick
import dimswitches

Item {
    signal settingsButtonPressed()
    signal settingsButtonReleased()
    signal outButtonPressed()
    signal outButtonReleased()
    signal errButtonPressed()
    signal errButtonReleased()
    signal f1ButtonPressed()
    signal f1ButtonReleased()
    signal f2ButtonPressed()
    signal f2ButtonReleased()
    signal leftRot()
    signal rightRot()
    signal pushRot()
    signal pushRotReleased()
    
    id: dimSwitchesParent

    property bool isSettingsButtonPressed: false
    property bool isOutButtonPressed: false
    property bool isErrButtonPressed: false
    property bool isF1ButtonPressed: false
    property bool isF2ButtonPressed: false
    property bool isPushRotButtonPressed: false

    Connections {
        target: DimSwitchEmitter
        function onSettingsButtonPressed() {
            settingsButtonPressed()
            dimSwitchesParent.isSettingsButtonPressed = true
        }
        function onSettingsButtonReleased() {
            settingsButtonReleased()
            dimSwitchesParent.isSettingsButtonPressed = false
        }
        function onOutButtonPressed() {
            outButtonPressed()
            dimSwitchesParent.isOutButtonPressed = true
        }
        function onOutButtonReleased() {
            outButtonReleased()
            dimSwitchesParent.isOutButtonPressed = false
        }
        function onErrButtonPressed() {
            errButtonPressed()
            dimSwitchesParent.isErrButtonPressed = true
        }
        function onErrButtonReleased() {
            errButtonReleased()
            dimSwitchesParent.isErrButtonPressed = false
        }
        function onF1ButtonPressed() {
            f1ButtonPressed()
            dimSwitchesParent.isF1ButtonPressed = true
        }
        function onF1ButtonReleased() {
            f1ButtonReleased()
            dimSwitchesParent.isF1ButtonPressed = false
        }
        function onF2ButtonPressed() {
            f2ButtonPressed()
            dimSwitchesParent.isF2ButtonPressed = true
        }
        function onF2ButtonReleased() {
            f2ButtonReleased()
            dimSwitchesParent.isF2ButtonPressed = false
        }
        function onLeftRot() {leftRot()}
        function onRightRot() {rightRot()}
        function onPushRot() {
            pushRot()
            dimSwitchesParent.isPushRotButtonPressed = true
        }
        function onPushRotReleased() {
            pushRotReleased()
            dimSwitchesParent.isPushRotButtonPressed = false
        }
    }
}