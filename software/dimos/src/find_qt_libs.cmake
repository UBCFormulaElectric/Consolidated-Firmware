# qt
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core Gui Widgets Qml Svg Quick)
if(${QT_VERSION_MAJOR} LESS 6)
    message("❌ Qt6 is required" FATAL_ERROR)
endif()
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Qml Svg Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets Qml Svg Quick)
list(APPEND REQUIRED_LIBRARIES Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Qml Qt6::Svg Qt6::Quick)
message("📚✔️ Added Qt6 libraries")