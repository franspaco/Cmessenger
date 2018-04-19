
#ifndef PROTOCOL_H
#define PROTOCOL_H

#define UNAME_LENGTH 16

// Server 2 client codes
typedef enum s2c_codes {
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
     * Trailer: %lu - found user id
     */
    USR_FND,

    /**
     * Queried user was not found
     */
    USR_NFND,

    /**
     * A message was received.
     * Trailer: %lu %s - src user id, message content
     */
    RCV_MSG,

    /**
     * Server is closing
     */
    S_QUIT
} s2c_t;

// Client 2 server codes
typedef enum c2s_codes {
    /**
     * Start new connection
     * Trailer: %s - username (limited to UNAME_LENGTH-1 chars)
     */
    C_START,

    /**
     * Query if a user exists.
     * Current:
     * Trailer: %lu
     * 
     * Ideal:
     * Trailer: %s
     */
    QRY_USR,

    /**
     * Send a message
     * Trailer: %lu %s - destination id, message content
     */
    SND_MSG,

    /**
     * Client is closing
     */
    C_QUIT
} c2s_t;


#endif