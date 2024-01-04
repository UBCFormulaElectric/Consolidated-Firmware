qt_add_library(Squircle STATIC)
target_link_libraries(Squircle PRIVATE Qt6::Quick Qt6::Gui)
qt_add_qml_module(Squircle
    URI squircle
    VERSION 1.0
    SOURCES squircle.cpp squircle.h
    RESOURCE_PREFIX /
)