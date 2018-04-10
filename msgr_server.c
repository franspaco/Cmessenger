

#include "server.h"

// Basic Functions
void usage(char * program);
void setupHandlers();
void exitHandler(int signal);

int main(int argc, char * argv[]){
    int connection_fd;

    printf("\n=== MESSENGER SERVER ===\n");

    // Check the correct arguments
    if (argc != 2) {
        usage(argv[0]);
    }

    // Show the IPs assigned to this computer
	printLocalIPs();

    // Start the server
    connection_fd = initServer(argv[1], MAX_QUEUE);;

    serverlog(INFO, "Server ready!");

    awaitConnections(connection_fd);

    // Close the socket
    close(connection_fd);

    return 0;
}

/*
    Explanation to the user of the parameters required to run the program
*/
void usage(char * program) {
    printf("Usage:\n");
    printf("\t%s {port_number}\n", program);
    exit(EXIT_FAILURE);
}

/*
    Modify the signal handlers for specific events
*/
void setupHandlers() {
    signal(SIGINT, exitHandler);
}


/**
 * SIGINT handler
 */
void exitHandler(int signal){
    activateExitFlag();
}