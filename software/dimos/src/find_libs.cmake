# OUTPUTS
# - REQUIRED_LIBRARIES -> list of libraries to link

# qt
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core Gui Widgets Qml Svg Quick)
if(${QT_VERSION_MAJOR} LESS 6)
    message("❌ Qt6 is required" FATAL_ERROR)
endif()
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Qml Svg Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets Qml Svg Quick)
list(APPEND REQUIRED_LIBRARIES Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Qml Qt6::Svg Qt6::Quick)
message("📚 Added Qt6 libraries")

#gpiod
if(${LINUX})
    find_library(GPIOD_LIBRARY NAMES libgpiod gpiod)
    if(NOT GPIOD_LIBRARY)
        message("${GPIOD_LIBRARY}")
        message(FATAL_ERROR "❌ gpiod library not found. Install sudo apt install libgpiod-dev")
    endif()
    list(APPEND REQUIRED_LIBRARIES "gpiod")
    list(APPEND REQUIRED_LIBRARIES "gpiodcxx")
    message("📚 Added gpiod library")
endif ()

# jsoncan
jsoncan_library(dimos_can dimos "${CMAKE_CURRENT_BINARY_DIR}/jsoncan" TRUE)
add_library(dimos_can STATIC ${CAN_SRCS})
target_include_directories(dimos_can PUBLIC ${CAN_INCLUDE_DIRS})
target_compile_definitions(dimos_can PRIVATE -DTHREAD_SAFE_CAN_PACKING)
list(APPEND REQUIRED_LIBRARIES "dimos_can")
message("📚 Added JsonCAN library")

# commit_info
set(COMMIT_INFO_SRC "${CMAKE_CURRENT_BINARY_DIR}/commit_info/App_CommitInfo.c")
set(COMMIT_INFO_HEADER "${CMAKE_CURRENT_BINARY_DIR}/commit_info/App_CommitInfo.h")
commit_info_register_library(${COMMIT_INFO_SRC} ${COMMIT_INFO_HEADER})
add_library(dimos_commit_info STATIC ${COMMIT_INFO_SRC})
target_include_directories(dimos_commit_info PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/commit_info")
list(APPEND REQUIRED_LIBRARIES "dimos_commit_info")
message("📚 Added commit_info library")