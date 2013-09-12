/**********************
 * ~~~ blockqueue.c ~~~
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
 
/**
 * This is a blocking queue, used to communicate between pthreads. 
 * Author:
 *		Shawn Paul Smith
 */

#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include "blockqueue.h"

//Adds toAdd to the queue, blocking if full.
void queue_put(const int toAdd, queue_t queue) {
    sem_wait(queue->empty_sem); //wait for room

    pthread_mutex_lock(queue->lock);
    queue->array[queue->tail] = toAdd;
    queue->tail = (queue->tail + 1) % queue->cap;
    pthread_mutex_unlock(queue->lock);

    sem_post(queue->size_sem); //announce new contents
}

//Returns the next item in the queue, blocking if empty.
int queue_get(queue_t queue) {
    sem_wait(queue->size_sem); //wait for contents

    pthread_mutex_lock(queue->lock);
    int val = queue->array[queue->head];
    queue->head = (queue->head + 1) % queue->cap;
    pthread_mutex_unlock(queue->lock);

    sem_post(queue->empty_sem); //announce room made
    return val;
}

// Attempts to add to the queue
// returns TRUE if sucsessful, FLASE if not
int queue_tryput(const int toAdd, queue_t queue) {
    if (sem_trywait(queue->empty_sem)) {
    	return FALSE;
    }
    pthread_mutex_lock(queue->lock);
    queue->array[queue->tail] = toAdd;
    queue->tail = (queue->tail + 1) % queue->cap;
    pthread_mutex_unlock(queue->lock);

    sem_post(queue->size_sem); //announce new contents
    return TRUE;
}

// Attempts to dequeue the next item in the queue, setting it in the given address
// returns TRUE if sucsessful, FLASE if not
int queue_tryget(int * value, queue_t queue) {
    if (sem_trywait(queue->size_sem)) {
    	return FALSE;
    }

    pthread_mutex_lock(queue->lock);
    *value = queue->array[queue->head];
    queue->head = (queue->head + 1) % queue->cap;
    pthread_mutex_unlock(queue->lock);

    sem_post(queue->empty_sem); //announce room made
    return TRUE;
}

/** Creates a new BlockingQueue of the given capacity **/
queue_t queue_init(const int capacity) {
    queue_t queue = malloc(sizeof(queue_str));
    queue->cap = capacity;
    queue->array = malloc(sizeof(int) * capacity);
    queue->head = 0;
    queue->tail = 0;
    queue->lock = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
    queue->empty_sem = (sem_t *) malloc(sizeof(sem_t));
    queue->size_sem = (sem_t *) malloc(sizeof(sem_t));
    pthread_mutex_init(queue->lock, NULL);
    sem_init(queue->empty_sem, 0, capacity);
    sem_init(queue->size_sem, 0, 0);
    return queue;
}

//Frees all memory associated with queue
void queue_destroy(queue_t queue) {
    free(queue->array);
    pthread_mutex_destroy(queue->lock);
    sem_destroy(queue->empty_sem);
    sem_destroy(queue->size_sem);
    free(queue->lock);
    free(queue->empty_sem);
    free(queue->size_sem);
    free(queue);
}
