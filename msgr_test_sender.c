

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
#include <pthread.h>
#include <time.h>
// Sockets libraries
#include <netdb.h>
#include <arpa/inet.h>
// poll
#include <sys/poll.h>
// Custom libraries
#include "sockets.h"
#include "fatal_error.h"
#include "protocol.h"

#define BUFFER_SIZE 1024

#define THREAD_POOL 20
#define MSG_COUNT 100

///// FUNCTION DECLARATIONS
void usage(char * program);
void* threads(void* arg);


void purgeStdin(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

typedef struct thread_data {
    int id;
    char* host;
    char* port;
    char* dest;
} tdata_t;

///// MAIN FUNCTION
int main(int argc, char * argv[]) {
    srand(time(NULL));
    // Check the correct arguments
    if (argc != 4) {
        usage(argv[0]);
    }

    pthread_t tids[THREAD_POOL];

    for(int i = 0; i < THREAD_POOL; i++){
        tdata_t* data = malloc(sizeof(tdata_t));
        data->id = i+1;
        data->host = argv[1];
        data->port = argv[2];
        data->dest = argv[3];
        pthread_create(&tids[i], NULL, threads, (void*)data);
        printf("[%i] CREATED.\n", i+1);
    }

    for(int i = 0; i < THREAD_POOL; i++){
        pthread_join(tids[i], NULL);
    }

    return 0;
}

/*
    Explanation to the user of the parameters required to run the program
*/
void usage(char * program)
{
    printf("Usage:\n");
    printf("\t%s {server_address} {port_number} {target username}\n", program);
    exit(EXIT_FAILURE);
}

char randChar(){
    return rand()%26 + 97;
}

void randStr(char* str, int size){
    for(int i = 0; i < size-1; i++){
        str[i] = randChar();
    }
    str[size-1] = '\0';
}

void threadCleanup(tdata_t* data, int fd){
    printf("[%i] DESTROYED.\n", data->id);
    // Close the socket
    close(fd);
    // Free data struct pased on to the thread
    free(data);
    pthread_exit(NULL);
}

void* threads(void* arg){
    //Set up
    long id;
    packet_t packet;
    tdata_t* data = (tdata_t*)arg;
    char uname[UNAME_LENGTH];
    sprintf(uname, "thread%02i", data->id);

    // Start conenction
    int fd = connectSocket(data->host, data->port);

    // Send start with username
    sendCodeStr(fd, C_START, uname);
    if(!readPacket(fd, &packet) && packet.code != REQ_OK){
        // error
        threadCleanup(data, fd);
    }

    sendCodeStr(fd, QRY_USR, data->dest);
    if(!readPacket(fd, &packet) && packet.code != USR_FND){
        // error
        threadCleanup(data, fd);
    }
    id = packet.id;

    char message[30];
    for(int i = 0; i < MSG_COUNT; i++){
        randStr(message, 30);
        sendCodeIdStr(fd, SND_MSG, id, message);
        printf("[%i] %s -> %li\n", data->id, message, id);
        if(!readPacket(fd, &packet)){
            threadCleanup(data, fd);
        }
        usleep(100000);
    }

    sendCode(fd, C_QUIT);

    threadCleanup(data, fd);
}
