# OUTPUTS
# - REQUIRED_LIBRARIES -> list of libraries to link

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core Gui Widgets Qml Svg Quick)
if(${QT_VERSION_MAJOR} LESS 6)
    message("❌ Qt6 is required" FATAL_ERROR)
endif()

message("💡 QT Version: ${QT_VERSION_MAJOR}")
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Qml Svg Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets Qml Svg Quick)
list(APPEND REQUIRED_LIBRARIES Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Qml Qt6::Svg Qt6::Quick)

if(${LINUX})
    find_library(GPIOD_LIBRARY NAMES libgpiod gpiod)
    if(NOT GPIOD_LIBRARY)
        message("${GPIOD_LIBRARY}")
        message(FATAL_ERROR "❌ gpiod library not found. Install sudo apt install libgpiod-dev")
    endif()
    list(APPEND REQUIRED_LIBRARIES "gpiod")
    list(APPEND REQUIRED_LIBRARIES "gpiodcxx")
endif ()

# jsoncan
jsoncan_library(dimos_can dimos "${CMAKE_BINARY_DIR}" TRUE)
add_library(dimos_can STATIC ${CAN_SRCS})
list(APPEND REQUIRED_LIBRARIES "dimos_can")
target_include_directories(dimos_can PUBLIC ${CAN_INCLUDE_DIRS})
target_compile_definitions(dimos_can PRIVATE -DTHREAD_SAFE_CAN_PACKING)