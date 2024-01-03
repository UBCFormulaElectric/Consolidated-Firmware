qt_add_library(DimosConstants STATIC)

set_target_properties(DimosConstants PROPERTIES AUTOMOC ON)
target_link_libraries(DimosConstants PUBLIC Qt6::Quick)

set_source_files_properties(constants.h PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt_add_qml_module(DimosConstants
    URI constants
    VERSION 1.0
    SOURCES constants.h
    RESOURCE_PREFIX /
)