qt_add_library(Components STATIC)
target_link_libraries(Components PRIVATE Qt6::Quick Qt6::Gui Qt6::Svg)
file(GLOB_RECURSE COMPONENT_SOURCES *.cpp *.h)
qt_add_qml_module(Components
        URI components
        VERSION 1.0
        SOURCES ${COMPONENT_SOURCES}
        QML_FILES Speedometer.qml
        RESOURCE_PREFIX /
)