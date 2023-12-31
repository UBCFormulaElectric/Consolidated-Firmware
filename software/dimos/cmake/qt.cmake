function(REGISTER_QML_TARGETS EXECUTABLE_NAME)
#    qt_add_qml_module(${EXECUTABLE_NAME}
#        URI hello
#        QML_FILES
#            main.qml
#        NO_RESOURCE_TARGET_PATH
#    )
endfunction()

function(ADD_QT_EXECUTABLE EXECUTABLE_NAME PROJECT_SOURCES LIBRARIES)
    message("➕ Adding build target ${EXECUTABLE_NAME}")

    # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
    qt_add_executable(${EXECUTABLE_NAME} MANUAL_FINALIZATION ${PROJECT_SOURCES})

    target_link_libraries(${EXECUTABLE_NAME} PRIVATE Qt6::Widgets Qt6::Core Qt6::Gui Qt6::Qml Qt6::Svg Qt6::Quick ${LIBRARIES})

    # Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
    # If you are developing for iOS or macOS you should consider setting an
    # explicit, fixed bundle identifier manually though.
    if (${QT_VERSION} VERSION_LESS 6.1.0)
        set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.${EXECUTABLE_NAME})
    endif ()
    set_target_properties(${EXECUTABLE_NAME} PROPERTIES
            ${BUNDLE_ID_OPTION}
            MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
            MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
            MACOSX_BUNDLE FALSE
            WIN32_EXECUTABLE TRUE
    )

    include(GNUInstallDirs)
    install(TARGETS ${EXECUTABLE_NAME}
            BUNDLE DESTINATION .
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

    REGISTER_QML_TARGETS(${EXECUTABLE_NAME})

    target_include_directories(${EXECUTABLE_NAME} PRIVATE src/shared src/ui src/io assets)
    target_compile_definitions(${EXECUTABLE_NAME} PUBLIC "USING_${EXECUTABLE_NAME}")

    qt_finalize_executable(${EXECUTABLE_NAME})
endfunction()