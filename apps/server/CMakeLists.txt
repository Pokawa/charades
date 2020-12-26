project(charades-server)
add_executable(server main.cpp Server.cpp Server.hpp)
target_link_libraries(server PRIVATE myLib spdlog::spdlog fmt::fmt)