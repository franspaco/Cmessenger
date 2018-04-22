
#ifndef RW_LIST_H
#define RW_LIST_H

#include <stdlib.h>
#include <pthread.h>

// Must be a pointer

#define TYPE void*


typedef struct node_struct {
    long id;
    TYPE data;
    struct node_struct* next;
} rw_list_node_t;

typedef struct list_struct {
    rw_list_node_t* root;
    pthread_rwlock_t rw_lock;
    long next_id;
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
 * Depending on value passed as 2nd argument it will:
 * 0 - clear thew list only
 * 1 - clear the list AND free the data
 */
void rw_list_clear(rw_list_t* list, int free_data);


/**
 * Get element at a certain index
 * Not fit for iteration on multi-threaded programs
 * Returns 1 on success
 * Returns 0 on failure
 */
int rw_list_get_element(rw_list_t* list, TYPE* dest, int index);

/**
 * Crete a new read-lock on the list.
 * Useful for iterating outside of the list
 * WARNING: the lock must be MANUALLY released 
 * for writing to occur.
 */
void rw_rdlock(rw_list_t* list);

/**
 * Allows to externally unlock a list.
 * Only use if the list was externally locked
 * for reading!
 */
void rw_unlock(rw_list_t* list);

#endif