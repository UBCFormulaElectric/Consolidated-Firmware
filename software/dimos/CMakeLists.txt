include(cmake/qt.cmake)

cmake_minimum_required(VERSION 3.24)
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
# define sources
set(PROJECT_SOURCES
        src/ui/main.cpp
        src/ui/mainwindow/mainwindow.cpp
        src/ui/mainwindow/mainwindow.h
        src/ui/mainwindow/mainwindow.ui
)
set(IO_INC src/io/can.h)
set(SHARED_FILES src/shared/types.cpp src/shared/types.h)

set(DEPLOY_IO_SOURCES src/io/can.cpp)
set(DEV_IO_SOURCES src/dev_io/can.cpp)


# jsoncan
jsoncan_library(dimos_can dimos "${CMAKE_BINARY_DIR}" TRUE)
add_library(dimos_can STATIC ${CAN_SRCS})
target_include_directories(dimos_can PUBLIC ${CAN_INCLUDE_DIRS})
target_compile_definitions(dimos_can PRIVATE -DTHREAD_SAFE_CAN_PACKING)

# define targets
set(DEV_SOURCES ${PROJECT_SOURCES} ${DEV_IO_SOURCES} ${IO_INC} ${SHARED_FILES})
#set(DEPLOY_SOURCES ${PROJECT_SOURCES} ${DEPLOY_IO_SOURCES} ${IO_INC} ${SHARED_FILES})

ADD_QT_EXECUTABLE(dimos_dev "${DEV_SOURCES}" "dimos_can")
#ADD_QT_EXECUTABLE(dimos "${DEPLOY_SOURCES}" "dimos_can")

