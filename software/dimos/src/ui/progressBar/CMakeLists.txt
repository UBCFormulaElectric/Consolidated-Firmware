qt_add_library(progressBar STATIC)
#target_link_libraries(Switcher PUBLIC Qt::Qml) # ubuntu libqt5qml-graphicaleffects
qt_add_qml_module(progressBar
        progress bar
        VERSION 1.0
        QML_FILES progressBar.qml
        RESOURCE_PREFIX /
)