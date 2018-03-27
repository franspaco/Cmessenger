/*
    Server program to handle a messenger like system in C
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Sockets libraries
#include <netdb.h>
#include <arpa/inet.h>

// Libraries for polling functionality
#include <sys/poll.h>
#include <errno.h>

// Custom libraries
#include <ifaddrs.h>

// Custom libraries
#include "fatal_error.h"

#include <signal.h>

#define BUFFER_SIZE 1024
#define MAX_QUEUE 5

///// FUNCTION DECLARATIONS
void usage(char * program);
void printLocalIPs();
int initServer(char * port);
void waitForConnections(int server_fd);
char* getUsername(int client_fd);


///// MAIN FUNCTION
int main(int argc, char * argv[]){
    int server_fd;

    printf("\n=== MESSENGER SERVER ===\n");

    // Check the correct arguments
    if (argc != 2)
    {
        usage(argv[0]);
    }

    // Show the IPs assigned to this computer
    printLocalIPs();

    // Start the server
    server_fd = initServer(argv[1]);
    // Listen for connections from the clients
    waitForConnections(server_fd);
    // Close the socket
    close(server_fd);

    return 0;
}

///// FUNCTION DEFINITIONS

/*
    Explanation to the user of the parameters required to run the program
*/
void usage(char * program)
{
    printf("Usage:\n");
    printf("\t%s {port_number}\n", program);
    exit(EXIT_FAILURE);
}

/*
 *   Show the local IP addresses, to allow testing
 *   Based on code from:
 *      https://stackoverflow.com/questions/20800319/how-do-i-get-my-ip-address-in-c-on-linux
 */
void printLocalIPs() {
    struct ifaddrs * addrs;
    struct ifaddrs * tmp;
    
    // Get the list of IP addresses used by this machine
    getifaddrs(&addrs);
    tmp = addrs;

    printf("Server IP addresses:\n");

    while (tmp) {
        if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_INET) {
            // Get the address structure casting as IPv4
            struct sockaddr_in *pAddr = (struct sockaddr_in *)tmp->ifa_addr;
            // Print the ip address of the local machine
            printf("%s: %s\n", tmp->ifa_name, inet_ntoa(pAddr->sin_addr));
        }
        // Advance in the linked list
        tmp = tmp->ifa_next;
    }

    freeifaddrs(addrs);
}

/*
 *  Prepare and open the listening socket
 *   Returns the file descriptor for the socket
 *   Remember to close the socket when finished
 */
int initServer(char * port) {
    struct addrinfo hints;
    struct addrinfo * server_info = NULL;
    int server_fd;
    int reuse = 1;

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
    if (getaddrinfo(NULL, port, &hints, &server_info) == -1) {
        fatalError("getaddrinfo");
    }

    // SOCKET
    // Open the socket using the information obtained
    server_fd = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
    if (server_fd == -1) {
        close(server_fd);
        fatalError("socket");
    }

    // SETSOCKOPT
    // Allow reuse of the same port even when the server does not close correctly
    if (setsockopt (server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof (int)) == -1) {
        fatalError("setsockopt");
    }

    // BIND
    // Connect the port with the desired port
    if (bind(server_fd, server_info->ai_addr, server_info->ai_addrlen) == -1) {
        fatalError("bind");
    }

    // LISTEN
    // Start listening for incomming connections
    if (listen(server_fd, MAX_QUEUE) == -1) {
        fatalError("listen");
    }

    // FREEADDRINFO
    // Free the memory used for the address info
    freeaddrinfo(server_info);

    printf("Server ready\n");

    return server_fd;
}

/*
 *   Main loop to wait for incomming connections
 */
void waitForConnections(int server_fd) {
    struct sockaddr_in client_address;
    socklen_t client_address_size;
    char client_presentation[INET_ADDRSTRLEN];
    int client_fd;
    pid_t new_pid;
    char buffer[BUFFER_SIZE];

    // Get the size of the structure to store client information
    client_address_size = sizeof client_address;

    while (1) {
        // ACCEPT
        // Wait for a client connection
        client_fd = accept(server_fd, (struct sockaddr *) &client_address, &client_address_size);
        if (client_fd == -1) {
            fatalError("accept");
        }

        int child_to_parent[2];
        // Open pipe of child to parent
        if (pipe(child_to_parent) == -1) {
            perror("Unable to open pipe");
            exit(EXIT_FAILURE);
        }
        
        // Create a child to deal with the new client
        new_pid = fork();

        // Parent process
        if (new_pid > 0) {
            // Close unwanted channel
            close(child_to_parent[1]);

            // Structure to indicate the sockets to poll
            struct pollfd test_fds[1];
            int poll_result;
            int timeout = 1000; // In milliseconds (1 sec)

            // POLL
            // Fill in the data for the structure
            test_fds[0].fd = server_fd;
            test_fds[0].events = POLLIN;

            poll_result = poll(test_fds, 1, timeout);

            if (poll_result == -1) {
                printf("[ERROR] [%i] Error polling pipe\n", getpid());
            } else if (poll_result == 1) {
                 if (test_fds[0].revents & POLLIN) {

                    // Read username from child
                    read(child_to_parent[0], buffer, BUFFER_SIZE);
                    printf("[INFO] [%i] The username is: %s\n", getpid(), buffer);
                }
            }

            // Close the socket to the new client
            close(client_fd);
            close(child_to_parent[0]);
            
        }

        // Child process
        else if (new_pid == 0) {
            // Close unwanted channel
            close(child_to_parent[0]);

            // Get the data from the client
            inet_ntop (client_address.sin_family, &client_address.sin_addr, client_presentation, sizeof client_presentation);
            printf("[INFO] [%i] Received incomming connection from %s on port %d\n",  getpid(), client_presentation, client_address.sin_port);
            
            // Deal with the client
            char* username = getUsername(client_fd);
            sprintf(buffer, "%s", username);
            write(child_to_parent[1], buffer, strlen(buffer)+1);

            // Finish the child process
            close(client_fd);

            close(child_to_parent[1]);
            exit(EXIT_SUCCESS);
        }
        // Error
        else {
            fatalError("Unable to fork");
        }
    }
}

/*
    Hear the request from the client and send an answer. 
    Get the username
*/
char* getUsername(int client_fd) {
    char buffer[BUFFER_SIZE];
    int chars_read;
    char username[15];
    double dummy;
    
    // Clear the buffer to avoid errors
    bzero(&buffer, BUFFER_SIZE);

    // RECV
    // Read the request from the client
    chars_read = recv(client_fd, buffer, BUFFER_SIZE, 0);
    if (chars_read == 0) {
        printf("[INFO] [%i] Client disconnected\n", getpid());
        return "";
    }
    if (chars_read == -1) {
        printf("[ERROR] [%i] Client receive error\n", getpid());
        return "";
    }
    // Get the numerical value for iterations
    sscanf(buffer, "%s", username);

    printf("[INFO] [%i] Got request from client with username=%s\n", getpid(), username);

    dummy = 1.5;

    // SEND
    // Write back the reply
    sprintf(buffer, "%lf\n", dummy);
    if (send(client_fd, buffer, strlen(buffer) + 1, 0) == -1) {
        printf("[ERROR] [%i] Could not send reply!\n", getpid());   
    }

    return username;
}




