cmake_minimum_required(VERSION 3.10)

project(DevPilot)

set(CMAKE_CXX_STANDARD 11)

find_package(CURL REQUIRED)

add_executable(devpilot main.cpp)

target_link_libraries(devpilot CURL::libcurl)
