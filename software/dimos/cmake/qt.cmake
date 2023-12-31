function(ADD_QT_EXECUTABLE EXECUTABLE_NAME PROJECT_SOURCES LIBRARIES)
    message("➕ Adding build target ${EXECUTABLE_NAME}")
    if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
        qt_add_executable(${EXECUTABLE_NAME} MANUAL_FINALIZATION ${PROJECT_SOURCES})
        # Define target properties for Android with Qt 6 as:
        #    set_property(TARGET Dimos APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
        #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
        # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
    else ()
        if (ANDROID)
            add_library(Dimos SHARED ${PROJECT_SOURCES})
            # Define properties for Android with Qt 5 after find_package() calls as:
            #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
        else ()
            add_executable(Dimos ${PROJECT_SOURCES})
        endif ()
    endif ()

    target_link_libraries(${EXECUTABLE_NAME}
            PRIVATE
            Qt${QT_VERSION_MAJOR}::Widgets
            Qt${QT_VERSION_MAJOR}::Core
            Qt${QT_VERSION_MAJOR}::Gui
            Qt${QT_VERSION_MAJOR}::Qml
            Qt${QT_VERSION_MAJOR}::Svg
            Qt${QT_VERSION_MAJOR}::Quick
            ${LIBRARIES})

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

    if (QT_VERSION_MAJOR EQUAL 6)
        qt_finalize_executable(${EXECUTABLE_NAME})
    endif ()

    target_include_directories(${EXECUTABLE_NAME} PRIVATE src/shared src/ui src/io assets)

    target_compile_definitions(${EXECUTABLE_NAME} PUBLIC "USING_${EXECUTABLE_NAME}")
endfunction()