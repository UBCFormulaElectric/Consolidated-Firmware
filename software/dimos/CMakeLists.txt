cmake_minimum_required(VERSION 3.24)
set(DIMOS_VERSION 0.1)
IF("${TARGET}" STREQUAL "dev")
    project("Dimos Local Dev" VERSION ${DIMOS_VERSION} LANGUAGES CXX)
ELSEIF ("${TARGET}" STREQUAL "deploy")
    project("Dimos Deploy" VERSION ${DIMOS_VERSION} LANGUAGES CXX)
ELSEIF ("${TARGET}" STREQUAL "test")
    project("Dimos Test" VERSION ${DIMOS_VERSION} LANGUAGES CXX)
ELSE()
    message(FATAL_ERROR "❌ Please set the TARGET variable to 'dev', 'deploy' or 'test'. To do this, set the -DTARGET=<target>")
ENDIF()

message("💡 Building ${CMAKE_PROJECT_NAME}")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets Core Gui QuickWidgets)
message("💡 QT Version: ${QT_VERSION_MAJOR}")
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core Gui QuickWidgets)

# todo use file glob to autodetect new files
set(PROJECT_SOURCES
        src/ui/main.cpp
        src/ui/mainwindow/mainwindow.cpp
        src/ui/mainwindow/mainwindow.h
        src/ui/mainwindow/mainwindow.ui
        src/shared/types.cpp src/shared/types.h
)

set(IO_Headers src/io/can.h)
IF("${TARGET}" STREQUAL "deploy")
    set(IO_Sources src/io/can.cpp)
ELSEIF("${TARGET}" STREQUAL "dev")
    set(IO_Sources src/dev_io/can.cpp)
ELSEIF("${TARGET}" STREQUAL "test")
ENDIF()

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Dimos
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
            ${IO_Headers}
            ${IO_Sources}
    )
    # Define target properties for Android with Qt 6 as:
    #    set_property(TARGET Dimos APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
    #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
    # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else ()
    if (ANDROID)
        add_library(Dimos SHARED
                ${PROJECT_SOURCES}
        )
        # Define properties for Android with Qt 5 after find_package() calls as:
        #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else ()
        add_executable(Dimos
                ${PROJECT_SOURCES}
        )
    endif ()
endif ()

target_link_libraries(Dimos
        PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::QuickWidgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if (${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Dimos)
endif ()
set_target_properties(Dimos PROPERTIES
        ${BUNDLE_ID_OPTION}
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Dimos
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if (QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Dimos)
endif ()

target_include_directories(Dimos PRIVATE src/shared src/io src/ui src/dev_io)