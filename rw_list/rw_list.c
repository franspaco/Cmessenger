
#include "rw_list.h"

#include <stdio.h>

/**
 * We use an id to identify each unique record
 */
long next_id;

rw_list_t* rw_list_init(){
    rw_list_t* temp = malloc(sizeof(rw_list_t));
    temp->root = NULL;
    pthread_rwlock_init(&(temp->rw_lock), NULL);
    return temp;
}

long rw_list_push_back(rw_list_t* list, TYPE new_data){
    pthread_rwlock_t* lock = &(list->rw_lock);
    rw_list_node_t* new_node = malloc(sizeof(rw_list_node_t));
    new_node->id = next_id++;
    new_node->next = NULL;
    new_node->data = new_data;

    pthread_rwlock_wrlock(lock);
    if(list->root == NULL){
        list->root = new_node;
    }
    else {
        rw_list_node_t* next_ptr = list->root;
        while(next_ptr->next != NULL){
            next_ptr = next_ptr->next;
        }
        next_ptr->next = new_node;
    }
    pthread_rwlock_unlock(lock);
    return new_node->id;
}

int rw_list_delete(rw_list_t* list, long id){
    pthread_rwlock_t* lock = &(list->rw_lock);
    pthread_rwlock_wrlock(lock);

    // In case list is empty
    if(list->root == NULL){
        pthread_rwlock_unlock(lock);
        return 0;
    }

    rw_list_node_t* curr = list->root;
    // In case the head is the desired item
    if(curr->id == id){
        list->root = curr->next;
        free(curr);
        pthread_rwlock_unlock(lock);
        return 1;
    }

    // set current as the second node
    curr = list->root->next;
    // init previous as the first node
    rw_list_node_t* prev = list->root;
    // Search for the key to be deleted while keeping track of the previous node
    while(curr != NULL){
        if(curr->id == id){
            // ID was found: delete
            prev->next = curr->next;
            free(curr);
            pthread_rwlock_unlock(lock);
            return 1;
        }
        prev = curr;
        curr = curr->next;
    }
    // Id not found
    pthread_rwlock_unlock(lock);
    return 0;
}

int rw_list_find(rw_list_t* list, TYPE* dest, long id){
    pthread_rwlock_t* lock = &(list->rw_lock);
    pthread_rwlock_rdlock(lock);
    rw_list_node_t* curr_ptr = list->root;
    while(curr_ptr != NULL){
        if(curr_ptr->id == id){
            *dest = curr_ptr->data;
            pthread_rwlock_unlock(lock);
            return 1;
        }
        curr_ptr = curr_ptr->next;
    }
    pthread_rwlock_unlock(lock);
    dest = NULL;
    return 0;
}

int rw_list_length(rw_list_t* list){
    int len = 0;
    pthread_rwlock_t* lock = &(list->rw_lock);
    pthread_rwlock_rdlock(lock);
    rw_list_node_t* next_ptr = list->root;
    while(next_ptr != NULL){
        len++;
        next_ptr = next_ptr->next;
    }
    pthread_rwlock_unlock(lock);
    return len;
}

void rw_list_clear(rw_list_t* list){

}