include (cmake/qt.cmake)
cmake_minimum_required(VERSION 3.24)
project("Dimos" VERSION 0.1 LANGUAGES CXX)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("💡 Building ${CMAKE_PROJECT_NAME}")

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

set(IO_INC src/io/can.h)
set(DEPLOY_IO_SOURCES src/io/can.cpp)
set(DEV_IO_SOURCES src/dev_io/can.cpp)

set(DEV_SOURCES ${PROJECT_SOURCES} ${DEV_IO_SOURCES} ${IO_INC})
set(DEPLOY_SOURCES ${PROJECT_SOURCES} ${DEPLOY_IO_SOURCES} ${IO_INC})

ADD_QT_EXECUTABLE(dimos_dev "${DEV_SOURCES}")
ADD_QT_EXECUTABLE(dimos "${DEPLOY_SOURCES}")