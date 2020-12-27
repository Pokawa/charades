project(charades-server)
add_executable(server Main.cpp ConnectionHandler.cpp ConnectionHandler.hpp PlayersHandler.cpp PlayersHandler.hpp Player.cpp Player.hpp)
target_link_libraries(server PRIVATE myLib spdlog::spdlog fmt::fmt)