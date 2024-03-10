qt_add_library(Components STATIC)
target_link_libraries(Components PRIVATE Qt6::Quick Qt6::Gui)
qt_add_qml_module(Components
        URI components
        VERSION 1.0
        SOURCES squircle.cpp squircle.h CircleProgressBar.cpp CircleProgressBar.h
        RESOURCE_PREFIX /
)