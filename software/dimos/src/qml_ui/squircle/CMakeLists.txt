qt_add_library(Squircle STATIC)

set_target_properties(Squircle PROPERTIES AUTOMOC ON)
target_link_libraries(Squircle PUBLIC Qt6::Quick)

qt_add_qml_module(Squircle
    URI squircle
    VERSION 1.0
    SOURCES squircle.cpp squircle.h
    RESOURCE_PREFIX /
)