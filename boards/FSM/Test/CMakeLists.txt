file(GLOB GOOGLETEST_TEST_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/Src/*.cpp")
list(REMOVE_ITEM GOOGLETEST_TEST_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/Src/main.cpp")
set(GOOGLETEST_MAIN_SRC "${CMAKE_CURRENT_SOURCE_DIR}/Src/main.cpp")

# Expose the following variables to the parent scope (i.e. The scope of any
# other CMakeLists.txt that uses add_subdirectory() on this CMakeLists.txt).
set(GOOGLETEST_TEST_SRCS ${GOOGLETEST_TEST_SRCS} PARENT_SCOPE)
set(GOOGLETEST_MAIN_SRC ${GOOGLETEST_MAIN_SRC} PARENT_SCOPE)
