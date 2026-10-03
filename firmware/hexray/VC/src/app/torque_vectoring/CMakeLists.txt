file(GLOB_RECURSE TV_SRCS CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${SHARED_APP_INCLUDE_DIR_CPP}/app_pid.cpp"
)

# MSVC's Debug /RTC checks cannot be combined with optimization. Keep this
# override local to the torque vectoring library's CMake directory.
if (MSVC)
    string(REGEX REPLACE "/RTC[1csu]+" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
endif ()

add_library("hexray_VC_torque_vectoring" STATIC ${TV_SRCS})
set_target_properties("hexray_VC_torque_vectoring" PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${TV_ARCHIVE_OUTPUT_DIR}")
target_include_directories("hexray_VC_torque_vectoring" PUBLIC "${INCLUDE_DIRS}")
target_link_libraries("hexray_VC_torque_vectoring" PUBLIC "autodiff_interface" "eigen_interface" "m")

# torque_allocator.cpp instantiates large Eigen/autodiff templates and can exceed
# MSVC's default COFF section limit.
if (MSVC)
    target_compile_options("hexray_VC_torque_vectoring" PRIVATE /MT /bigobj /O2 /Ob2)
else ()
    target_compile_options("hexray_VC_torque_vectoring" PRIVATE -O2)
endif ()

# Simulink links the Debug build of this library, which has no NDEBUG, so Eigen would bounds-check every
# coefficient access. The allocator only uses fixed-size matrices, so those asserts buy nothing here.
target_compile_definitions("hexray_VC_torque_vectoring" PRIVATE EIGEN_NO_DEBUG)
