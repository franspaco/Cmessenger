
#ifndef RW_LIST_H
#define RW_LIST_H

#include <stdlib.h>
#include <pthread.h>

// Must be a pointer
#define TYPE int*

typedef struct node_struct {
    long id;
    TYPE data;
    struct node_struct* next;
} rw_list_node_t;

typedef struct list_struct {
    rw_list_node_t* root;
    pthread_rwlock_t rw_lock;
} rw_list_t;

/**
 * Returns a ready to use pointer to a list
 */
rw_list_t* rw_list_init();

/**
 * Adds data to the back of the list
 * returns the ID assigned to the element
 */
long rw_list_push_back(rw_list_t* list, TYPE new_data);

/**
 * Deletes a node with a given ID from a list
 *  Warning: Does nothing to the contained data
 */
int rw_list_delete(rw_list_t* list, long id);


/**
 * Finds the data associated with one node based on its id
 * Sets dest to point to the data contained in the node
 * Returns 1 if the id was found, 0 if it wasn't
 */
int rw_list_find(rw_list_t* list, TYPE* dest, long id);

/**
 * Returns the length of a list
 */
int rw_list_length(rw_list_t* list);

/**
 * Frees all the nodes in a list and the list itself
 * Does NOT free the data pointers
 */
void rw_list_clear(rw_list_t* list);


#endif