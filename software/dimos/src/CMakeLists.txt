# FROM THE PARENT SCOPE, WE INHERIT
# - EXECUTABLE_NAME
include(find_qt_libs.cmake) # we need to do this super early because this provides Qt CMake functions
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
# define sources
set(UI_SOURCES main.cpp setup.cpp assets/resources.qrc)
file(GLOB SHARED_FILES "${CMAKE_CURRENT_SOURCE_DIR}/shared/*.cpp")
file(GLOB TASKS_FILES "${CMAKE_CURRENT_SOURCE_DIR}/tasks/*.cpp")
if ("${TARGET}" STREQUAL "deploy")
    file(GLOB_RECURSE IO_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/io/*.cpp")
elseif ("${TARGET}" STREQUAL "dev")
    file(GLOB_RECURSE IO_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/dev_io/*.cpp")
endif ()
set(SOURCES ${UI_SOURCES} ${IO_SOURCES} ${SHARED_FILES} ${TASKS_FILES} io/KeyTranslator.h)

# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
qt_add_executable(${EXECUTABLE_NAME} ${SOURCES})
# include directories
target_include_directories(${EXECUTABLE_NAME} PRIVATE shared io dev_io assets tasks .)
# link libraries
include(find_libs.cmake)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE ${REQUIRED_LIBRARIES})
# to tell which configuration it is from macros
target_compile_definitions(${EXECUTABLE_NAME} PUBLIC "USING_TARGET_${TARGET}")
target_compile_definitions(${EXECUTABLE_NAME} PRIVATE -DQT_DEBUG -DQT_MESSAGELOGCONTEXT) # W mans https://stackoverflow.com/a/43549795/6833423
target_compile_definitions(${EXECUTABLE_NAME} PRIVATE QT_QML_DEBUG)
# QML
set_target_properties(${EXECUTABLE_NAME} PROPERTIES AUTOMOC ON AUTORCC ON)
qt_add_qml_module(${EXECUTABLE_NAME}
        URI "DimosMain"
        VERSION 1.0
        QML_FILES ui/MainWindow.qml
)
# add qml modules
add_subdirectory(ui)
target_link_libraries(${EXECUTABLE_NAME} PUBLIC
        DimosConstantsplugin
        Switcherplugin
        Pagesplugin
        Componentsplugin
        DimSwitchesplugin
        CanQMLplugin
)
target_compile_definitions(DimosConstants PUBLIC "USING_TARGET_${TARGET}") # L QML compiler
target_link_libraries(CanQML PUBLIC dimos_can)

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