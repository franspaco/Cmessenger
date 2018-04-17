#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <pthread.h>

void struct QueueHeader* createQueue();

typedef struct QueueElem {
  void *next;
  void *value;
} QueueElem;

typedef struct QueueHeader {
  QueueElem *head;
  QueueElem *tail;
  pthread_mutex_t *mutex;
} QueueHeader;


#endif