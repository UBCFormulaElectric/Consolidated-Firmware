message("")
message("Configuring matplotlib-cpp...")

# Optional: matplotlib-cpp needs the Python development headers/libraries. If they are not installed, skip the target
# (and anything that links it) instead of failing the whole configure.
find_package(Python3 COMPONENTS Development)
if (NOT Python3_Development_FOUND)
    message(STATUS "Python3 development files not found. Skipping matplotlib-cpp (plotting tests will not be built).")
    return()
endif ()

# Library target
add_library(matplotlib_cpp INTERFACE)
target_include_directories(matplotlib_cpp INTERFACE ${matplotlibcpp_SOURCE_DIR})
target_link_libraries(matplotlib_cpp INTERFACE
        Python3::Python
        Python3::Module
)
#find_package(Python3 COMPONENTS NumPy)
#if (Python3_NumPy_FOUND)
#    target_link_libraries(matplotlib_cpp INTERFACE
#            Python3::NumPy
#    )
#else ()
message(WARNING "NumPy not found. Matplotlib-cpp will be built without NumPy support.")
target_compile_definitions(matplotlib_cpp INTERFACE WITHOUT_NUMPY)
#endif ()
