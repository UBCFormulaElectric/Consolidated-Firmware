
# Find all the include directories for shared libraries
file(GLOB SHARED_INCLUDE_DIRS LIST_DIRECTORIES true CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/*/Inc
        ${CMAKE_CURRENT_SOURCE_DIR}
        )

# Find all the source files for shared libraries
file(GLOB SHARED_SRCS LIST_DIRECTORIES false CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/*/Src/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/*/Src/*.cpp
        )

# Make the two variables we just created visible in the parent scope, so that
# any cmake file that includes this one via `add_subdirectory` can see them
set(SHARED_INCLUDE_DIRS ${SHARED_INCLUDE_DIRS} PARENT_SCOPE)
set(SHARED_SRCS ${SHARED_SRCS} PARENT_SCOPE)

