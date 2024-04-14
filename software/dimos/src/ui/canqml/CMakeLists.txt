qt_add_library(CanQML STATIC
        canqml.cpp)
qt_add_qml_module(CanQML
    URI canqml
    VERSION 1.0
    SOURCES canqml.h canqml.cpp
    RESOURCE_PREFIX /
)