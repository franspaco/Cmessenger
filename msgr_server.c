

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

    // Init the client list
    rw_list_t* client_list = rw_list_init();

    awaitConnections(connection_fd, client_list);

    // Close the socket
    close(connection_fd);

    //TODO: delete all nodes in client list
    // Free the client list
    free(client_list);

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