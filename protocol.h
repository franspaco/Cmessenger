
#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "sockets.h"

// Max allowed username length
#define UNAME_LENGTH 16

// Max allowed packet size
#define SOCK_BUFF_SIZE 1024

// Struct to contain info received
typedef struct packet_struct {
    int code;
    long id;
    char msg[1024];
} packet_t;


typedef enum protocol_struct {
/// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
///                                   SERVER TO CLIENT
/// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    /**
     * Last request was OK
     */
    REQ_OK,

    /**
     * Last request raised an ERROR
     */
    REQ_ERR,

    /**
     * Queried user was found
     * Trailer: %li - found user id
     */
    USR_FND,

    /**
     * Queried user was not found
     */
    USR_NFND,

    /**
     * A message was received.
     * Trailer: %li %s - src user id, message content
     */
    RCV_MSG,

    /**
     * Server is closing
     */
    S_QUIT,

/// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
///                                   CLIENT TO SERVER
/// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

    /**
     * Start new connection
     * Trailer: %s - username (limited to UNAME_LENGTH-1 chars)
     */
    C_START,

    /**
     * Query if a user exists.
     * Current:
     * Trailer: %li
     * 
     * Ideal:
     * Trailer: %s
     */
    QRY_USR,

    /**
     * Send a message
     * Trailer: %li %s - destination id, message content
     */
    SND_MSG,

    /**
     * Client is closing
     */
    C_QUIT
} protocol_t;

/**
 * Gets data received by the socket, uses protocol to determine how
 *  to parse it and fills in the packet passed to the function.
 */
int readPacket(int connection_fd, packet_t* packet);

/**
 * Send code
 */
void sendCode(int connection_fd, int code);

/**
 * Send a code followed by a string
 */
void sendCodeStr(int connection_fd, int code, char* trailer);

/**
 * Send a code, and id and a string
 */
void sendCodeIdStr(int connection_fd, int code, long id, char* trailer);


#endif