message("")
message("Configuring matplotlib-cpp...")

# Library target
find_package(Python3 COMPONENTS Development REQUIRED)
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