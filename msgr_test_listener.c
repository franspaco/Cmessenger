

/**
 * Francisco Huelsz Prince
 * A01019512
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Custom libraries
#include "protocol.h"

#define BUFFER_SIZE 1024

///// FUNCTION DECLARATIONS
void usage(char * program);
void* threads(void* arg);


void purgeStdin(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void cleanup(int fd){
    close(fd);
}

///// MAIN FUNCTION
int main(int argc, char * argv[]) {
    // Check the correct arguments
    if (argc != 4) {
        usage(argv[0]);
    }

    packet_t packet0;
    char uname[UNAME_LENGTH];
    sprintf(uname, "%s", argv[3]);

    // Start conenction
    int fd = connectSocket(argv[1], argv[2]);

    // Send start with username
    sendCodeStr(fd, C_START, uname);
    if(!readPacket(fd, &packet0) && packet0.code != REQ_OK){
        cleanup(fd);
    }
    int count = 0;
    while(1){
        packet_t packet;
        readPacket(fd, &packet);
        //printf("Got: %i %li %s\n", packet.code, packet.id, packet.msg);
        printf("Tot: %i\n", ++count);
    }
    cleanup(fd);
    return 0;
}

/*
    Explanation to the user of the parameters required to run the program
*/
void usage(char * program) {
    printf("Usage:\n");
    printf("\t%s {server_address} {port_number} {target username}\n", program);
    exit(EXIT_FAILURE);
}