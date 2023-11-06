function(ADD_QT_EXECUTABLE executable_name PROJECT_SOURCES)
    message("➕ Adding build target ${executable_name}")
    if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
        qt_add_executable(${executable_name} MANUAL_FINALIZATION ${PROJECT_SOURCES})
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

    target_link_libraries(${executable_name}
            PRIVATE
            Qt${QT_VERSION_MAJOR}::Widgets
            Qt${QT_VERSION_MAJOR}::Core
            Qt${QT_VERSION_MAJOR}::Gui
            Qt${QT_VERSION_MAJOR}::QuickWidgets)

    # Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
    # If you are developing for iOS or macOS you should consider setting an
    # explicit, fixed bundle identifier manually though.
    if (${QT_VERSION} VERSION_LESS 6.1.0)
        set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.${executable_name})
    endif ()
    set_target_properties(${executable_name} PROPERTIES
            ${BUNDLE_ID_OPTION}
            MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
            MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
            MACOSX_BUNDLE TRUE
            WIN32_EXECUTABLE TRUE
    )

    include(GNUInstallDirs)
    install(TARGETS ${executable_name}
            BUNDLE DESTINATION .
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

    if (QT_VERSION_MAJOR EQUAL 6)
        qt_finalize_executable(${executable_name})
    endif ()

    target_include_directories(${executable_name} PRIVATE src/shared src/ui src/io)
endfunction()