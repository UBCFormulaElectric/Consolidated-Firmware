include("/home/amirt/Consolidated-Firmware/build_x86/cmake/CPM.cmake")
CPMAddPackage("NAME;LITTLEFS;GITHUB_REPOSITORY;littlefs-project/littlefs;GIT_TAG;v2.8.0")
set(LITTLEFS_FOUND TRUE)