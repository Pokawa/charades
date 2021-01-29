cmake_minimum_required(VERSION 3.12)
project(charades)

set(CMAKE_CXX_STANDARD 17)

find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)
find_package(Threads REQUIRED)


add_subdirectory(src)
add_subdirectory(apps/server)
add_subdirectory(apps/client)
