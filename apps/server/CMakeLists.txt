project(charades-server)
add_executable(server Main.cpp ConnectionHandler.cpp ConnectionHandler.hpp PlayersHandler.cpp PlayersHandler.hpp Player.cpp Player.hpp IOHandler.cpp IOHandler.hpp Room.cpp Room.hpp RoomsHandler.cpp RoomsHandler.hpp CharadesWordsPool.cpp CharadesWordsPool.hpp)
target_link_libraries(server PRIVATE myLib spdlog::spdlog fmt::fmt)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/words.txt
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})