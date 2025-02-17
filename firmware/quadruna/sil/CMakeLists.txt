IF("${TARGET}" STREQUAL "sil")
        file(GLOB_RECURSE SRCS 
                "${CMAKE_CURRENT_SOURCE_DIR}/src/manager/*.c"
                "${CMAKE_CURRENT_SOURCE_DIR}/src/manager/*.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/*.c"
                "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/*.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/../CRIT/io/*.c"
                "${CMAKE_CURRENT_SOURCE_DIR}/../CRIT/io/*.cpp"
s        ) 

        add_executable(quadruna_sil "${SRCS}")
        target_include_directories(
                quadruna_sil PRIVATE 
                "${CMAKE_CURRENT_SOURCE_DIR}/src/manager" 
                "${CMAKE_CURRENT_SOURCE_DIR}/src/shared"
                "${CMAKE_CURRENT_SOURCE_DIR}/../CRIT/io"
                "${CMAKE_CURRENT_SOURCE_DIR}/../../shared/src/io"
                "${CMAKE_CURRENT_SOURCE_DIR}/../../shared/src/app"
        )

        # Add CZMQ and math lib
        target_link_libraries (quadruna_sil ${CZMQ_LIBRARIES} m)
        target_include_directories(quadruna_sil PRIVATE "${CZMQ_INCLUDE_DIRS}")
ENDIF()