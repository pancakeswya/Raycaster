cmake_minimum_required(VERSION 3.22)
project(untitled1)

find_package(SFML 2.5 REQUIRED system window graphics)

set(CMAKE_CXX_STANDARD 17)

add_executable(untitled1 main.cc game.cc player.cc)

target_link_libraries(untitled1 -lsfml-graphics -lsfml-window -lsfml-system)

target_include_directories(untitled1 PUBLIC ${CMAKE_INCLUDE_CURRENT_DIR})
target_include_directories(untitled1 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})