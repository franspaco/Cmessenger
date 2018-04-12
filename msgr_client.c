

/**
 * Francisco Huelsz Prince
 * A01019512
 */

/*
    Client program to access the accounts in the bank
    This program connects to the server using sockets

    Gilberto Echeverria
    gilecheverria@yahoo.com
    29/03/2018
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// Sockets libraries
#include <netdb.h>
#include <arpa/inet.h>
// poll
#include <sys/poll.h>
// Custom libraries
#include "sockets.h"
#include "fatal_error.h"

#define BUFFER_SIZE 1024

///// FUNCTION DECLARATIONS
void usage(char * program);
void client_ops(int fd);

void purgeStdin(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

///// MAIN FUNCTION
int main(int argc, char * argv[])
{
    int connection_fd;

    printf("\n=== BANK CLIENT PROGRAM ===\n");

    // Check the correct arguments
    if (argc != 3)
    {
        usage(argv[0]);
    }

    // Start the server
    connection_fd = connectSocket(argv[1], argv[2]);
	

    client_ops(connection_fd);
    

    // Close the socket
    close(connection_fd);

    return 0;
}

/*
    Explanation to the user of the parameters required to run the program
*/
void usage(char * program)
{
    printf("Usage:\n");
    printf("\t%s {server_address} {port_number}\n", program);
    exit(EXIT_FAILURE);
}

void client_ops(int fd){
    char buffer[1024];

    // Stuff required by poll stdin
    struct pollfd test_fds[1];
    int timeout = 10; // 10ms tiemout
    int poll_result;

    char input = 0;
    while(input != 'x'){
        printf("OPTs:\n s - send\n x - exit\n");
        while(1){
            test_fds[0].fd = stdin->_fileno;
            test_fds[0].events = POLLIN;
            poll_result = poll(test_fds, 1, timeout);
            if(poll_result == -1){
                //??
                fatalError("STDIN poll");
            }
            else if(poll_result == 0){
                // Nothing: check for server replies
                test_fds[0].fd = fd;
                test_fds[0].events = POLLIN;
                poll_result = poll(test_fds, 1, timeout);
                if(poll_result == -1){
                    //??
                    fatalError("Server poll");
                }
                else if(poll_result == 0){
                    // Nothing: continue polling
                    continue;
                }
                else{
                    // Got something, most likely a BYE: read and make sure
                    // Receive the response
                    if (recvString(fd, buffer, BUFFER_SIZE) ){
                        printf("\n\nServer sent: %s\n", buffer);
                        //exit(0);
                    }
                    else {
                        exit(0);
                    }
                }
            }
            else{
                scanf(" %c", &input);
                break;
            }
        }

        switch(input){
            case 'x':
                exit(0);
            case 's':
                printf("Write:\n");
                purgeStdin();
                fgets(buffer, 1024, stdin);
                printf("sending: %s\n", buffer);
                sendString(fd, buffer);
        }
    }
}