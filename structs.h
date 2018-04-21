
#ifndef STRUCTS_H
#define STRUCTS_H

#include "rw_list/rw_list.h"

typedef struct thread_data{
    int id; // Thread id for debugging purposes
    int fd; // Client file descriptor
    rw_list_t* clients;
    QueueHeader* queue; // Queue of messages
    
} tdata_t;

typedef struct message_struct{
    long source_id;
    //char* source_uanme;
    char* content;
} msg_t;


#endif