
cmake_minimum_required (VERSION 2.6)

set(SERVER "TS_SERVER")
set(CLIENT "TS_CLIENT")

project("${NAME}")

set(CMAKE_C_STANDARD 11)

add_executable("${SERVER}" msgr_server.c server.h server.c sockets.h sockets.c fatal_error.h fatal_error.c protocol.h structs.h)
target_link_libraries("${SERVER}" pthread)

add_executable("${CLIENT}" msgr_client.c sockets.h sockets.c fatal_error.h fatal_error.c protocol.h structs.h)
target_link_libraries("${CLIENT}" pthread)