#include "queue.h"

QueueHeader* createQueue() {
  struct QueueHeader *handle = malloc(sizeof(*handle));
  handle->head = NULL;
  handle->tail = NULL;

  pthread_mutex_t *mutex = malloc(sizeof(*mutex));
  handle->mutex = mutex;
  
  return handle;
}

void destroy(struct QueueHeader *header) {
  free(header->mutex);
  free(header);
  header = NULL;
}

void push(struct QueueHeader *header, void *elem) {
  // Create new element
  QueueElem *element = malloc(sizeof(*element));
  element->value = elem;
  element->next = NULL;

  pthread_mutex_lock(header->mutex);
  // Is list empty
  if (header->head == NULL) {
  	header->head = element;
  	header->tail = element;
  } else {
  	// Rewire
  	struct QueueElem* oldTail = header->tail;
  	oldTail->next = element;
  	header->tail = element;
  }
  pthread_mutex_unlock(header->mutex);
}

void* pop(struct QueueHeader *header) {
  pthread_mutex_lock(header->mutex);
  QueueElem *head = header->head;

  // Is empty?
  if (head == NULL) {
  	pthread_mutex_unlock(header->mutex);
  	return NULL;
  } else {
	  // Rewire
  	header->head = head->next;
  	
  	// Get head and free element memory
  	void *value = head->value;
  	free(head);
  	
  	pthread_mutex_unlock(header->mutex);
  	return value;
  }
}

