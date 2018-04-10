
#ifndef SERVER_H_lol
#define SERVER_H_lol

// POLL
#include <sys/poll.h>

// ERRORS
#include <errno.h>

// SIGNALS
#include <signal.h>

// Posix threads library
#include <pthread.h>

// Custom libraries
#include "protocol.h"
#include "sockets.h"
#include "fatal_error.h"
#include "structs.h"

// SERVER DEFINITIONS
#define BUFFER_SIZE 1024
#define MAX_QUEUE 10

// Server enums
typedef enum log_types {
    INFO,
    ERROR,
    WARN,
    DEBUG,
    ALERT
} log_t;


// Public function declaration

/**
 * Main server function
 */
void awaitConnections(int server_fd);

/**
 * Activate the exit flag to end the server
 */
void activateExitFlag();

/**
 * Helper function for printing debug/log messages
 */
void serverlog(log_t type, char* msg);

#endif