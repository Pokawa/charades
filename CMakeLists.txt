cmake_minimum_required(VERSION 3.17)
project(charades)

set(CMAKE_CXX_STANDARD 17)

find_package(spdlog)

add_subdirectory(src)
add_subdirectory(apps/server)
