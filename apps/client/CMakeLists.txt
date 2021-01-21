cmake_minimum_required(VERSION 3.5)

project(charades-client LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools REQUIRED)

set(TS_FILES client_en_150.ts)

add_executable(client
    Main.cpp
        ConnectDialog.cpp
        ConnectDialog.h
        ConnectDialog.ui
        ${TS_FILES}
        )

target_link_libraries(client PRIVATE Qt${QT_VERSION_MAJOR}::Widgets myLib spdlog::spdlog fmt::fmt)

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
