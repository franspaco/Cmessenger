
cmake_minimum_required (VERSION 3.0)
set(CMAKE_C_STANDARD 11)

set(SERVER "msg_server")
set(CLIENT "msg_client")

set(FATAL_ERROR fatal_error.h fatal_error.c)
set(SOCKETS sockets.h sockets.c)
set(RW_LIST rw_list/rw_list.h rw_list/rw_list.c)
set(UTILS protocol.h structs.h queue.h queue.c)

set(SERVER_SOURCES server.h server.c ${UTILS} ${FATAL_ERROR} ${RW_LIST} ${SOCKETS})
set(CLIENT_SOURCES ${UTILS} ${FATAL_ERROR} ${SOCKETS})

add_executable("${SERVER}" msgr_server.c ${SERVER_SOURCES})
target_link_libraries("${SERVER}" pthread)

add_executable("${CLIENT}" msgr_client.c ${CLIENT_SOURCES})
target_link_libraries("${CLIENT}" pthread)