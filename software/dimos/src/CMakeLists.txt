# FROM THE PARENT SCOPE, WE INHERIT
# - EXECUTABLE_NAME
include(find_qt_libs.cmake) # we need to do this super early because this provides Qt CMake functions
set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
# define sources
set(UI_SOURCES main.cpp assets/resources.qrc)
file(GLOB IO_INC "${CMAKE_CURRENT_SOURCE_DIR}/io/*.h")
file(GLOB SHARED_FILES "${CMAKE_CURRENT_SOURCE_DIR}/shared/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/shared/*.cpp")
file(GLOB TASKS_FILES "${CMAKE_CURRENT_SOURCE_DIR}/tasks/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/tasks/*.cpp")
if("${TARGET}" STREQUAL "deploy")
    file(GLOB_RECURSE IO_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/io/*.cpp")
elseif ("${TARGET}" STREQUAL "dev")
    file(GLOB_RECURSE IO_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/dev_io/*.cpp")
endif()
set(SOURCES ${UI_SOURCES} ${IO_SOURCES} ${IO_INC} ${SHARED_FILES} ${TASKS_FILES})

# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
qt_add_executable(${EXECUTABLE_NAME} ${SOURCES})
# include directories
target_include_directories(${EXECUTABLE_NAME} PRIVATE shared io assets tasks)
# link libraries
include(find_libs.cmake)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE ${REQUIRED_LIBRARIES})
# to tell which configuration it is from macros
target_compile_definitions(${EXECUTABLE_NAME} PUBLIC "USING_${EXECUTABLE_NAME}") # todo change to using_target_dimos
# QML
set_target_properties(${EXECUTABLE_NAME} PROPERTIES AUTOMOC ON AUTORCC ON)
qt_add_qml_module(${EXECUTABLE_NAME}
    URI "MainWindow"
    VERSION 1.0
    QML_FILES qml_ui/MainWindow.qml
)
#qt_query_qml_module(
#    ${EXECUTABLE_NAME}
#    URI uri_var
#    VERSION version_var
#    PLUGIN_TARGET plugin_target_var
#    TARGET_PATH target_path_var
#    MODULE_RESOURCE_PATH module_resource_path_var
#    QMLDIR qmldir_var
#    TYPEINFO typeinfo_var
#    QML_FILES qml_files_var
#    QML_FILES_DEPLOY_PATHS qml_files_deploy_paths_var
#    QML_FILES_PREFIX_OVERRIDES qml_files_prefix_overrides_var
#    RESOURCES resources_var
#    RESOURCES_DEPLOY_PATHS resources_deploy_paths_var
#    RESOURCES_PREFIX_OVERRIDES resources_prefix_overrides_var
#)
#
#message(
#"URI: ${uri_var}
#TARGET_PATH: ${target_path_var}
#MODULE_RESOURCE_PATH: ${module_resource_path_var}
#QMLDIR: ${qmldir_var}
#TYPEINFO: ${typeinfo_var}
#QML_FILES: ${qml_files_var}
#QML_FILES_DEPLOY_PATHS: ${qml_files_deploy_paths_var}"
#)
# add qml modules
add_subdirectory(qml_ui)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE DimosConstantsplugin Switcherplugin Pagesplugin Squircleplugin)
target_compile_definitions(DimosConstants PUBLIC "USING_${EXECUTABLE_NAME}") # L QML compiler

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
#    if (${QT_VERSION} VERSION_LESS 6.1.0)
#        set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.${EXECUTABLE_NAME})
#    endif ()
#    set_target_properties(${EXECUTABLE_NAME} PROPERTIES
#            ${BUNDLE_ID_OPTION}
#            MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
#            MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
#            MACOSX_BUNDLE FALSE
#            WIN32_EXECUTABLE TRUE
#    )
#    include(GNUInstallDirs)
#    install(TARGETS ${EXECUTABLE_NAME}
#            BUNDLE DESTINATION .
#            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#    )