cmake_minimum_required(VERSION 3.23)
project(TicTacToe)

set(CMAKE_CXX_STANDARD 23)

add_executable(client game_client.c board.h)
add_executable(server game_server.c board.h)
add_executable(bot bot_client.c bot_logic.c bot_logic.h)

