include(cmake/qt.cmake)

cmake_minimum_required(VERSION 3.24)
#set(CMAKE_AUTOUIC OFF)
#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-fdiagnostics-color=always")

message("💡 Building ${CMAKE_PROJECT_NAME}")
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core Gui Widgets Qml Svg Quick)
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    message("💡 QT Version: ${QT_VERSION_MAJOR}")
    find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Qml Svg Quick)
else()
    message("❌ Qt6 is required" FATAL_ERROR)
endif()
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets Qml Svg Quick)

qt_standard_project_setup()

if(${LINUX})
    find_library(GPIOD_LIBRARY NAMES libgpiod gpiod)
    if(NOT GPIOD_LIBRARY)
        message("${GPIOD_LIBRARY}")
        message(FATAL_ERROR "❌ gpiod library not found. Install sudo apt install libgpiod-dev")
    endif()
    list(APPEND REQUIRED_LIBRARIES "gpiod")
    list(APPEND REQUIRED_LIBRARIES "gpiodcxx")
endif ()

# define sources
file(GLOB_RECURSE UI_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/assets/resources.qrc")
file(GLOB_RECURSE SHARED_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/*.cpp")

file(GLOB_RECURSE IO_INC "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.h")

file(GLOB_RECURSE DEPLOY_IO_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.cpp")
file(GLOB_RECURSE DEV_IO_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/dev_io/*.cpp")


# jsoncan
jsoncan_library(dimos_can dimos "${CMAKE_BINARY_DIR}" TRUE)
add_library(dimos_can STATIC ${CAN_SRCS})
list(APPEND REQUIRED_LIBRARIES "dimos_can")
target_include_directories(dimos_can PUBLIC ${CAN_INCLUDE_DIRS})
target_compile_definitions(dimos_can PRIVATE -DTHREAD_SAFE_CAN_PACKING)

# define targets
set(INCLUDE_DIRS src/shared src/qml_ui src/io assets)
if("${TARGET}" STREQUAL "deploy")
    set(DEPLOY_SOURCES ${UI_SOURCES} ${DEPLOY_IO_SOURCES} ${IO_INC} ${SHARED_FILES})
    ADD_QT_EXECUTABLE(dimos "${DEPLOY_SOURCES}" "${INCLUDE_DIRS}" "${REQUIRED_LIBRARIES}")
elseif ("${TARGET}" STREQUAL "dev")
    set(DEV_SOURCES ${UI_SOURCES} ${DEV_IO_SOURCES} ${IO_INC} ${SHARED_FILES})
    ADD_QT_EXECUTABLE(dimos_dev "${DEV_SOURCES}" "${INCLUDE_DIRS}" "${REQUIRED_LIBRARIES}")
else()
    message(FATAL_ERROR "❌ Unknown target ${TARGET}")
endif()