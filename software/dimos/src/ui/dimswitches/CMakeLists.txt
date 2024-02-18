qt_add_library(DimSwitches STATIC)
target_link_libraries(DimSwitches PRIVATE Qt6::Quick Qt6::Gui)
qt_add_qml_module(DimSwitches
    URI dimswitches
    VERSION 1.0
    SOURCES DimSwitchEmitter.h DimSwitchEmitter.cpp
#    QML_FILES DimSwitches.qml
    RESOURCE_PREFIX /
)