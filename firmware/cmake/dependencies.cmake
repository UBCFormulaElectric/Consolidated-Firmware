message("")
message("📦 Handling Dependencies")

# Git Submodules
execute_process(COMMAND "git submodule update --init --recursive" WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
message("  ✅ Git Submodules Updated")
