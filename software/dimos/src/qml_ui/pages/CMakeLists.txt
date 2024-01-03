qt_add_library(Pages STATIC)

file(GLOB Pages_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.qml)

qt_add_qml_module(Pages
        URI pages
        VERSION 1.0
        QML_FILES ${Pages_SRC}
        RESOURCE_PREFIX /
)