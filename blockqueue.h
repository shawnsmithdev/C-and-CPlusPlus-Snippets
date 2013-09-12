/**********************
 * ~~~ blockqueue.h ~~~
 *
 * === Team Venture ===
 *  Jacob Church
 *  Ryan Erickson
 *  Shawn Smith
 *  Jason Mercado
 *
 * Project Due:
 * June 9, 2011
 **********************/

#ifndef BLOCKQUEUE_H
#define BLOCKQUEUE_H

#define TRUE -1
#define FALSE 0

#include <pthread.h>
#include <semaphore.h>

/**
 * queue_t is a Blocking Queue, implemented on an array.
 * It is equivelant to a pointer to the queue_str struct.
 * "queue_str const * queue" is like a queue_t that cannot be used to modify it.
 */
typedef struct queue_tag {
    int *array; //The circular array
    int head; //The first element of contents
    int tail; //The next empty element
    int cap;   //The size of the circular array
    pthread_mutex_t *lock; //a mutex on this queue
    sem_t *size_sem; //the number of elements, == size
    sem_t *empty_sem; //the room left in the array, cap - size
} queue_str, *queue_t;

//Adds toAdd to the queue, blocking if full.
void queue_put(const int toAdd, queue_t queue);

//Returns the next item in the queue, blocking if empty.
int queue_get(queue_t queue);

//Attempts to add an item to the queue.
int queue_tryput(const int toAdd, queue_t queue);

//Attempts to get the next item in the queue
int queue_tryget(int * value, queue_t queue);

/** Creates a new BlockingQueue of the given capacity **/
queue_t queue_init(const int capacity);

//Frees all memory associated with queue
void queue_destroy(queue_t queue);

#endif
