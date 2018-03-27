/*
    Client program to chat with your friends
    This program connects to the server using sockets.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Sockets libraries
#include <netdb.h>
#include <arpa/inet.h>

// Libraries for polling functionality
#include <errno.h>
#include <sys/poll.h>

// Library to handle signal
#include <signal.h>

// Custom libraries
#include "fatal_error.h"

#define BUFFER_SIZE 1024

///// FUNCTION DECLARATIONS
void usage(char * program);
int openSocket(char * address, char * port);
int establishUsername(int connection_fd);


///// MAIN FUNCTION
int main(int argc, char * argv[]) {
    printf("\n=== Cmessenger - The Messenger of the Future ===\n");

    // Check the correct arguments
    if (argc != 3) {
        usage(argv[0]);
    }

    // Start the server
    int connection_fd = openSocket(argv[1], argv[2]);

    // Establish username
    if(establishUsername(connection_fd)) {
        printf("Username saved ok?\n");
    }
    else {
        printf("There was an error when writing the username\n");
    }
    
    // Close the socket
    close(connection_fd);
    printf("Good bye!\n");

    return 0;
}

///// FUNCTION DEFINITIONS

/*
    Explanation to the user of the parameters required to run the program
*/
void usage(char * program) {
    printf("Usage:\n");
    printf("\t%s {server_address} {port_number}\n", program);
    exit(EXIT_FAILURE);
}

/*
    Open the socket to the server
    Returns the file descriptor for the socket
    Remember to close the socket when finished
*/
int openSocket(char * address, char * port) {
    struct addrinfo hints;
    struct addrinfo * server_info = NULL;

    // Prepare the hints structure
    bzero (&hints, sizeof hints);
    // Set to use IPv4
    hints.ai_family = AF_INET;
    // Set type of socket
    hints.ai_socktype = SOCK_STREAM;
    // Set to look for the address automatically
    hints.ai_flags = AI_PASSIVE;

    // GETADDRINFO
    // Use the presets to get the actual information for the socket
    if (getaddrinfo(address, port, &hints, &server_info) == -1) {
        fatalError("getaddrinfo");
    }

    // SOCKET
    // Open the socket using the information obtained
    int connection_fd = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
    if (connection_fd == -1) {
        close(connection_fd);
        fatalError("socket");
    }

    // CONNECT
    // Connect to the server

    if (connect(connection_fd, server_info->ai_addr, server_info->ai_addrlen) == -1) {
        fatalError("connect");
    }


    // FREEADDRINFO
    // Free the memory used for the address info
    freeaddrinfo(server_info);

    return connection_fd;
}

/*
    Tells the server the username for the current user
*/
int establishUsername(int connection_fd) {
    char buffer[BUFFER_SIZE];
    int chars_read;
    char username[15];

    printf("Enter a username: ");
    scanf("%s", username);

    // Prepare the request to the server
    sprintf(buffer, "%s\n", username);
    
    // SEND
    // Send the username to the server
    if (send(connection_fd, buffer, strlen(buffer) + 1, 0) == -1 )
    {
        fatalError("send");
    }
    
    // Clear the buffer
    bzero(&buffer, BUFFER_SIZE);

    // RECV
    // Receive the response from server
    chars_read = recv(connection_fd, buffer, BUFFER_SIZE, 0);
    if (chars_read == -1)
    {
        fatalError("recv");
    }

    int result;
    sscanf(buffer, "%d", &result);

    return result;
}

