
#ifndef STRUCTS_H
#define STRUCTS_H

#include "rw_list/rw_list.h"
#include "queue/queue.h"

typedef struct thread_data{
    int id; // Thread id for debugging purposes
    int fd; // Client file descriptor
    rw_list_t* clients;
    //TODO: add queue
} tdata_t;

typedef struct message_struct{
    long source_id;
    char* content;
} msg_t;

typedef struct client_data_struct{
    char uname[UNAME_LENGTH];
    QueueHeader* queue;
}client_data_t;


#endif