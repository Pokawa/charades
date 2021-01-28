project(charades-server)
add_executable(server Main.cpp ConnectionHandler.cpp ConnectionHandler.hpp PlayersHandler.cpp PlayersHandler.hpp Player.cpp Player.hpp IOHandler.cpp IOHandler.hpp Room.cpp Room.hpp RoomsHandler.cpp RoomsHandler.hpp CharadesWordsPool.cpp CharadesWordsPool.hpp LogicHandler.cpp LogicHandler.hpp)
target_link_libraries(server PRIVATE myLib spdlog::spdlog fmt::fmt)
