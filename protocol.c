

#include "protocol.h"

/// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
///                               PRIVATE FUNCTIONS
/// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

/**
 * Retrieves the code sent in a message
 */
int getCode(char * msg){
    int code;
    sscanf(msg, "%i", &code);
    return code;
}

/// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
///                                   PUBLIC FUNCTIONS
/// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

/**
 * Gets data received by the socket, uses protocol to determine how
 *  to parse it and fills in the packet passed to the function.
 */
int readPacket(int connection_fd, packet_t* packet){
    char buffer[SOCK_BUFF_SIZE];
    bzero(buffer, SOCK_BUFF_SIZE);
    int received = recvString(connection_fd, buffer, SOCK_BUFF_SIZE);
    if(received == 0){
        return 0;
    }
    int code = getCode(buffer);
    packet->code = code;
    switch(code){
        case RCV_MSG:   // s2c
            // Trailer: %li %s
            sscanf(buffer, "%i %li %[^\n]", &code, &(packet->id), packet->msg);
            break;
        case SND_MSG:   // c2s
            // Trailer: %li %s
            sscanf(buffer, "%i %li %[^\n]", &code, &(packet->id), packet->msg);
            break;
        case QRY_USR:   // c2s
            // Trailer: %s
            sscanf(buffer, "%i %s", &code, packet->msg);
            //sscanf(buffer, "%i %li", &code, &(packet->id));
            break;
        case QRY_USR_ID:// c2s
            // Trailer: %li
            sscanf(buffer, "%i %li", &code, &(packet->id));
            break;
        case USR_FND:   // s2c
            // Trailer: %li
            sscanf(buffer, "%i %li %s", &code, &(packet->id), packet->msg);
            break;
        case C_START:   // c2s
            // Trailer: %s
            sscanf(buffer, "%i %s", &code, packet->msg);
            break;
        case REQ_OK:    // s2c
            // Trailer: none
        case REQ_ERR:   // s2c
            // Trailer: none
        case USR_NFND:  // s2c
            // Trailer: none
        case S_QUIT:    // s2c
            // Trailer: none
        case C_QUIT:    // c2s
            // Trailer: none
            break;
        default:
            // very bad error
            fatalError("Invalid code received!");
    }
    return 1;
}

/**
 * Send code
 */
void sendCode(int connection_fd, int code){
    char buffer[SOCK_BUFF_SIZE];
    sprintf(buffer, "%i", code);
    sendString(connection_fd, buffer);
}

/**
 * Send a code followed by a string
 */
void sendCodeStr(int connection_fd, int code, char* trailer){
    char buffer[SOCK_BUFF_SIZE];
    sprintf(buffer, "%i %s", code, trailer);
    sendString(connection_fd, buffer);
}

/**
 * Send a code, and id and a string
 */
void sendCodeIdStr(int connection_fd, int code, long id, char* trailer){
    char buffer[SOCK_BUFF_SIZE];
    sprintf(buffer, "%i %li %s", code, id, trailer);
    sendString(connection_fd, buffer);
}