import QtQuick
import dimswitches

Item {
    signal settingButtonPressed()
    signal outButtonPressed()
    signal errButtonPressed()
    signal f1ButtonPressed()
    signal f2ButtonPressed()
    signal leftRot()
    signal rightRot()
    signal pushRot()

    Connections {
        target: DimSwitchEmitter
        function onSettingsButtonPressed() {settingButtonPressed()}
        function onOutButtonPressed() {outButtonPressed()}
        function onErrButtonPressed() {errButtonPressed()}
        function onF1ButtonPressed() {f1ButtonPressed()}
        function onF2ButtonPressed() {f2ButtonPressed()}
        function onLeftRot() {leftRot()}
        function onRightRot() {rightRot()}
        function onPushRot() {pushRot()}
    }
}