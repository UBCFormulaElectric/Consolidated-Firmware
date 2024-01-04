cmake_minimum_required(VERSION 3.24)
#set(CMAKE_AUTOUIC OFF)
#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("💡 Building ${CMAKE_PROJECT_NAME}")

add_subdirectory(src)