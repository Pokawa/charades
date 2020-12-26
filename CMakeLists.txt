cmake_minimum_required(VERSION 3.12)
project(charades)

set(CMAKE_CXX_STANDARD 17)

find_package(spdlog)
find_package(fmt)

add_subdirectory(src)
add_subdirectory(apps/server)
