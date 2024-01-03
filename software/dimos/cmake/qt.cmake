function(ADD_QT_EXECUTABLE EXECUTABLE_NAME PROJECT_SOURCES PROJECT_INC_DIRS LIBRARIES)
    message("➕ Adding build target ${EXECUTABLE_NAME}")

    # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
    qt_add_executable(${EXECUTABLE_NAME} ${PROJECT_SOURCES})
    target_link_libraries(${EXECUTABLE_NAME} PRIVATE Qt6::Widgets Qt6::Core Qt6::Gui Qt6::Qml Qt6::Svg Qt6::Quick ${LIBRARIES})

    set_target_properties(${EXECUTABLE_NAME} PROPERTIES AUTOMOC ON AUTORCC ON)

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

    qt_add_qml_module(${EXECUTABLE_NAME}
            URI "MainWindow"
            VERSION 1.0
            NO_PLUGIN
            QML_FILES src/qml_ui/MainWindow.qml
            NO_RESOURCE_TARGET_PATH
    )

    add_subdirectory(src/qml_ui)
    target_link_libraries(${EXECUTABLE_NAME} PRIVATE DimosConstantsplugin)

    target_include_directories(${EXECUTABLE_NAME} PRIVATE ${PROJECT_INC_DIRS})
    target_compile_definitions(${EXECUTABLE_NAME} PUBLIC "USING_${EXECUTABLE_NAME}") # todo change to using_target_dimos
endfunction()