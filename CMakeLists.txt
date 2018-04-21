
cmake_minimum_required (VERSION 3.0)
set(CMAKE_C_STANDARD 11)

set(SERVER "msg_server")
set(CLIENT "msg_client")
set(TEST_CLIENT "msg_test_client")

set(FATAL_ERROR fatal_error.h fatal_error.c)
set(SOCKETS sockets.h sockets.c)
set(RW_LIST rw_list/rw_list.h rw_list/rw_list.c)
set(UTILS protocol.h protocol.c structs.h)

set(SERVER_SOURCES server.h server.c ${UTILS} ${FATAL_ERROR} ${RW_LIST} ${SOCKETS})
set(CLIENT_SOURCES client.h client.c ${UTILS} ${FATAL_ERROR} ${RW_LIST} ${SOCKETS})
set(TEST_CLIENT_SOURCES ${UTILS} ${FATAL_ERROR} ${RW_LIST} ${SOCKETS})

add_executable("${SERVER}" msgr_server.c ${SERVER_SOURCES})
target_link_libraries("${SERVER}" pthread)

add_executable("${CLIENT}" msgr_client.c ${CLIENT_SOURCES})
target_link_libraries("${CLIENT}" pthread ncurses panel)

add_executable("${TEST_CLIENT}" msgr_test_client.c ${TEST_CLIENT_SOURCES})
target_link_libraries("${TEST_CLIENT}" pthread)