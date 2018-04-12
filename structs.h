
#ifndef STRUCTS_H
#define STRUCTS_H

#include "rw_list/rw_list.h"

typedef struct thread_data{
    int id; // Thread id for debugging purposes
    int fd; // Client file descriptor
    rw_list_t* clients;
    //TODO: add queue
} tdata_t;


#endif