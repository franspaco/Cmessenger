

#include "server.h"

int exit_flag = 0;

// SERVER LOG HELPER STUFF
const char * const log_types_strings[] = { "INFO", "ERROR", "WARN", "DEBUG", "ALERT"};
void conn_log(log_t type, tdata_t* data, char* msg){
    printf("[%s][%i] %s\n", log_types_strings[type], data->id, msg);
}
void serverlog(log_t type, char* msg){
    printf("[%s] %s\n", log_types_strings[type], msg);
}

/// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
///                               PRIVATE FUNCTIONS
/// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *


void* attendClient(void* arg){
    tdata_t* data = (tdata_t*) arg;

    // Stuff required by poll
    struct pollfd test_fds[1];
    int timeout = 10; // 10ms tiemout
    int poll_result;

    // Reception buffer
    char buffer[BUFFER_SIZE];

    while(!exit_flag){
        // POLL
        // Fill in the data for the structure
        test_fds[0].fd = data->fd;
        test_fds[0].events = POLLIN;
        // Call poll
        poll_result = poll(test_fds, 1, timeout);
        
        if (poll_result == -1) {
            // SIGINT will trigger this
            // errno is checked to make sure it was a signal that got us here and not an error
            if(errno == EINTR && exit_flag){
                printf("POLL KILLED BY FLAG!\n");
                // An exit signal got us here, therefore we must break
                break;
            }
            else{
                // Some other error got us here
                fatalError("poll");
            }
        }
        else if (poll_result == 0){
            // Nothing
        }
        else {
            // Got a message
            if(recvString(data->fd, buffer, BUFFER_SIZE) == 0){
                conn_log(INFO, data, "Client disconnected.");
                break;
            }
            conn_log(INFO, data, "GOT TEXT:");
            printf("\t%s\n", buffer);
            }
    }

    conn_log(INFO, data, "Closing handler.");
    free(data);
    pthread_exit(NULL);
}

/// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
///                                   PUBLIC FUNCTIONS
/// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

void activateExitFlag(){
    exit_flag = 1;
}

void awaitConnections(int server_fd){
    struct sockaddr_in client_address;
    socklen_t client_address_size;
    char client_presentation[INET_ADDRSTRLEN];
    int client_fd;
    pthread_t new_tid;
    int poll_response;
	int timeout = 100;		// Time in milliseconds (0.5 seconds)

    // Thread id counter for debug purposes
    int next_id = 100;

    // Get the size of the structure to store client information
    client_address_size = sizeof client_address;

    while (1)
    {
		//// POLL
        // Create a structure array to hold the file descriptors to poll
        struct pollfd test_fds[1];
        // Fill in the structure
        test_fds[0].fd = server_fd;
        test_fds[0].events = POLLIN;    // Check for incomming data
        // Check if there is any incomming communication
        poll_response = poll(test_fds, 1, timeout);

		// Error when polling
        if (poll_response == -1)
        {
            // SIGINT will trigger this
            // errno is checked to make sure it was a signal that got us here and not an error
            if(errno == EINTR && exit_flag){
                printf("\n\n[ALERT] SERVER SHUTTING DOWN! - No longer listening.\n");
                // An exit signal got us here, therefore we must break
                break;
            }
            else{
                // Some other error got us here
                fatalError("poll");
            }
        }
		// Timeout finished without reading anything
        else if (poll_response == 0){

            // The exit flag has been activated => stop listening for requests
            if(exit_flag){
                printf("\n\n[ALERT] SERVER SHUTTING DOWN! - No longer listening.\n");
                break;
            }
            //printf("No response after %d seconds\n", timeout);
        }
		// There is something ready at the socket
        else
        {
            // Check the type of event detected
            if (test_fds[0].revents & POLLIN)
            {
				// ACCEPT
				// Wait for a client connection
				client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_address_size);
				if (client_fd == -1)
				{
					fatalError("ERROR: accept");
				}
				 
				// Get the data from the client
				inet_ntop(client_address.sin_family, &client_address.sin_addr, client_presentation, sizeof client_presentation);

				// Prepare the structure to send to the thread
                tdata_t* temp_data = malloc(sizeof(tdata_t));
                temp_data->id = next_id++; // Assign id and increment
                temp_data->fd = client_fd;

				// CREATE A THREAD
                int status;
                status = pthread_create(&new_tid, NULL, attendClient, (void*) temp_data);
                if( status != 0){
                    printf("[ERROR] Failed to create handler for %s:%d.\n", client_presentation, client_address.sin_port);
                }
                else{
                    printf("\n[INFO] Created handler [%i] for request from %s:%d.\n", temp_data->id, client_presentation, client_address.sin_port);
                }
            }
        }
    }
}