#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <pthread.h>

typedef struct QueueElem {
  void *next;
  void *value;
} QueueElem;

typedef struct QueueHeader {
  QueueElem *head;
  QueueElem *tail;
  pthread_mutex_t mutex;
} QueueHeader;

QueueHeader* createQueue();
void q_push(struct QueueHeader *header, void *elem);
void* q_pop(struct QueueHeader *header);

#endif