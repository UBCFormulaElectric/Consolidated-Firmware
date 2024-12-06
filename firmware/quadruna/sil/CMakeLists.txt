IF("${TARGET}" STREQUAL "sil")
        file(GLOB_RECURSE SRCS 
                "${CMAKE_CURRENT_SOURCE_DIR}/src/manager/*.c"
                "${CMAKE_CURRENT_SOURCE_DIR}/src/manager/*.cpp"
        )

        add_executable(quadruna_sil "${SRCS}")

        # Add CZMQ.
        target_link_libraries (quadruna_sil ${CZMQ_LIBRARIES})
        target_include_directories(quadruna_sil PRIVATE "${CZMQ_INCLUDE_DIRS}")
ENDIF()