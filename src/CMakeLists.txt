add_library(myLib WebSocket.cpp Message.cpp)
target_include_directories(myLib PUBLIC ../include)
target_link_libraries(myLib PRIVATE spdlog::spdlog)
target_compile_features(myLib PUBLIC cxx_std_17)

