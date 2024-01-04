qt_add_library(Switcher STATIC)
qt_add_qml_module(Switcher
    URI switcher
    VERSION 1.0
    QML_FILES Switcher.qml
    RESOURCE_PREFIX /
)