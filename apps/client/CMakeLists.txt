project(charades-client)
add_executable(client Main.cpp)
target_link_libraries(client PRIVATE myLib spdlog::spdlog fmt::fmt)