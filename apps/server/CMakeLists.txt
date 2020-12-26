project(charades-server)
add_executable(server main.cpp)
target_link_libraries(server PRIVATE myLib spdlog::spdlog)