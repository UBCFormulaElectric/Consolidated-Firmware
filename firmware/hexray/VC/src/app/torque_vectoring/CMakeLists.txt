file(GLOB_RECURSE TV_SRCS CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${SHARED_APP_INCLUDE_DIR_CPP}/app_pid.cpp"
)

add_library("hexray_VC_torque_vectoring" STATIC ${TV_SRCS})
set_target_properties("hexray_VC_torque_vectoring" PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${TV_ARCHIVE_OUTPUT_DIR}")
target_include_directories("hexray_VC_torque_vectoring" PUBLIC "${INCLUDE_DIRS}")
target_link_libraries("hexray_VC_torque_vectoring" PUBLIC "autodiff_interface" "eigen_interface" "m")

target_compile_options("hexray_VC_torque_vectoring" PRIVATE -O2)

# Remove Eigen Assertions as they bloat the binary a lot, our matricies are fixed size anyways
target_compile_definitions("hexray_VC_torque_vectoring" PRIVATE EIGEN_NO_DEBUG)

# SIMULINK SHARED LIBRARY: cmake --build build_fw_test --target tv_controls_sim
# The Simulink C Function blocks call this library only through the extern "C" functions in
# torque_vectoring_matlab.h. Building it as a shared library with the GCC runtime linked in means MATLAB's MEX
# compiler (MinGW-w64 on Windows, Xcode Clang on macOS) never has to link C++ objects from a different compiler
# version: it only links the C import library (Windows) or the .dylib (macOS). EXCLUDE_FROM_ALL: it is only built
# when asked for by name, not by a normal test build. It keeps -O2 in this Debug build, so the sim runs fast.
set(TV_SIM_OUTPUT_DIR "${CMAKE_BINARY_DIR}/controls_sim")
add_library("tv_controls_sim" SHARED EXCLUDE_FROM_ALL ${TV_SRCS})
set_target_properties("tv_controls_sim" PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${TV_SIM_OUTPUT_DIR}"
        LIBRARY_OUTPUT_DIRECTORY "${TV_SIM_OUTPUT_DIR}"
        ARCHIVE_OUTPUT_DIRECTORY "${TV_SIM_OUTPUT_DIR}"
        # Windows: tv_controls_sim.dll with the import library tv_controls_sim.lib
        PREFIX ""
        IMPORT_PREFIX ""
        IMPORT_SUFFIX ".lib"
        # macOS: record the absolute path as the install name, so the MEX file finds the .dylib without an rpath
        INSTALL_NAME_DIR "${TV_SIM_OUTPUT_DIR}"
        BUILD_WITH_INSTALL_NAME_DIR ON)
target_include_directories("tv_controls_sim" PRIVATE "${INCLUDE_DIRS}")
target_link_libraries("tv_controls_sim" PRIVATE "autodiff_interface" "eigen_interface" "m")
target_compile_options("tv_controls_sim" PRIVATE -O2)
target_compile_definitions("tv_controls_sim" PRIVATE EIGEN_NO_DEBUG)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if (WIN32)
        # Link libgcc, libstdc++ and winpthread into the DLL, so it does not need the MinGW DLLs on the PATH
        target_link_options("tv_controls_sim" PRIVATE -static)
    else ()
        target_link_options("tv_controls_sim" PRIVATE -static-libgcc -static-libstdc++)
    endif ()
endif ()
