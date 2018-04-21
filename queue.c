/*
 * Implementation of thread-safe queue. It supports push and pop
 * operations
 */

#include "queue.h"

/*
 * Initializes the queue and its mutex
 *
 */
QueueHeader* createQueue() {
  struct QueueHeader *handle = malloc(sizeof(*handle));
  handle->head = NULL;
  handle->tail = NULL;

  pthread_mutex_t *mutex = malloc(sizeof(*mutex));
  handle->mutex = mutex;
  
  return handle;
}

/*
 * Destroys the queue pointer.
 */
void destroy(struct QueueHeader *header) {
  free(header->mutex);
  free(header);
  header = NULL;
}

/*
 * Pushes an element to the queue in a thread-safe way.
 */
void push(struct QueueHeader *header, void *elem) {
  // Create new element
  QueueElem *element = malloc(sizeof(*element));
  element->value = elem;
  element->next = NULL;

  pthread_mutex_lock(header->mutex);

  // Checks if list is empty
  if (header->head == NULL) {
  	header->head = element;
  	header->tail = element;
  } else {
  	// Change the tail
  	struct QueueElem* oldTail = header->tail;
  	oldTail->next = element;
  	header->tail = element;
  }
  pthread_mutex_unlock(header->mutex);
}

/*
 * Dequeues an element from the queue
 */
void* pop(struct QueueHeader *header) {
  pthread_mutex_lock(header->mutex);
  QueueElem *head = header->head;

  // Checks if list is empty
  if (head == NULL) {
  	pthread_mutex_unlock(header->mutex);
  	return NULL;
  } else {
	  // Change the head
  	header->head = head->next;
  	
  	// Get head and free element memory
  	void *value = head->value;
  	free(head);
  	
  	pthread_mutex_unlock(header->mutex);
  	return value;
  }
}

