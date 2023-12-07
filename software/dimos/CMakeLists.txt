include(cmake/qt.cmake)

cmake_minimum_required(VERSION 3.24)
set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("💡 Building ${CMAKE_PROJECT_NAME}")
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core Gui Widgets)
message("💡 QT Version: ${QT_VERSION_MAJOR}")
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets)

# define sources
file(GLOB_RECURSE PROJECT_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ui/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ui/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/assets/resources.qrc")
file(GLOB_RECURSE SHARED_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/*.cpp")

file(GLOB_RECURSE IO_INC "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.h")

file(GLOB_RECURSE DEPLOY_IO_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.cpp")
file(GLOB_RECURSE DEV_IO_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/dev_io/*.cpp")


# jsoncan
jsoncan_library(dimos_can dimos "${CMAKE_BINARY_DIR}" TRUE)
add_library(dimos_can STATIC ${CAN_SRCS})
target_include_directories(dimos_can PUBLIC ${CAN_INCLUDE_DIRS})
target_compile_definitions(dimos_can PRIVATE -DTHREAD_SAFE_CAN_PACKING)

# define targets
set(DEV_SOURCES ${PROJECT_SOURCES} ${DEV_IO_SOURCES} ${IO_INC} ${SHARED_FILES})
set(DEPLOY_SOURCES ${PROJECT_SOURCES} ${DEPLOY_IO_SOURCES} ${IO_INC} ${SHARED_FILES})

ADD_QT_EXECUTABLE(dimos_dev "${DEV_SOURCES}" "dimos_can")
ADD_QT_EXECUTABLE(dimos "${DEPLOY_SOURCES}" "dimos_can")