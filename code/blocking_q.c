#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include "blocking_q.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#define TODO printf("TODO!\n");

/**
 * Internal function to blocking_q. Takes an element
 * in the queue. This functions assumes the following
 * preconditions:
 *  - The thread has safe access to the queue
 *  - The queue is NOT empty
 * Also update the size.
 * @param q the queue
 * @return an element
 */
task_ptr __blocking_q_take(blocking_q *q) { // NOLINT(bugprone-reserved-identifier)
    TODO
    return NULL;
}

/**
 * Create a blocking queue. Initializes the synchronisation primitives
 * and
 * @param q the queue
 * @return if init was successful.
 */
bool blocking_q_init(blocking_q *q) {
    TODO
    return false;
}

/**
 * Destroy a blocking queue. Removes the allocations of the data
 * and destroys the sync. primitives.
 * @param q ptr to the blocking queue
 */
void blocking_q_destroy(blocking_q *q) {
    TODO
}

/**
 * Put a task in the blocking queue. This task can fail if no
 * memory is available to allocate a new entry in the queue
 * @param q the queue
 * @param data the data description to put inside the queue
 * @returns if the data was put correctly inside the queue.
 */
bool blocking_q_put(blocking_q *q, task_ptr data) {
    TODO
}

/**
 * Get an element in the blocking queue. If the queue is empty,
 * the current thread is put to sleep until an element is added
 * to the queue.
 * @param q the blocking queue
 * @return the element
 */
task_ptr blocking_q_get(blocking_q *q) {
    // using `while` instead of `if` to avoid some problems
    while (q->sz == 0) {
        /* Wait for elements
        -> - blocks the thread until the specified condition is signalled.
           - mutex should be locked and then automatically release while wait
           - must be unlocked once finished
        */
        pthread_cond_wait(&q->cond, &q->lock);
    }
    task_ptr element = __blocking_q_take(q);

    if (q->sz == 0) pthread_mutex_lock(&q->lock);

    return element;

}

/**
 * Drain as many elements as possible into the area allowed
 * by the pointer. This function does not block.
 * @param q the queue
 * @param data the pointer where to store the data
 * @param sz the maximum area available in the buffer
 * @return the number of entries written.
 */
size_t blocking_q_drain(blocking_q *q, task_ptr *data, size_t sz) {

    if (q->sz == 0) return 0;

    int counter = 0;
    data = malloc(sizeof(task_ptr) * sz);

    while (q->sz > 0)
        data[counter++] = __blocking_q_take(q);

    pthread_mutex_lock(&q->lock);
}

/**
 * Drain at least min elements in the buffer. This function
 * might block if there are not enough elements to drain.
 * @param q the queue
 * @param data the pointer where to store the data
 * @param sz the maximum area available in the buffer
 * @param min the minimum amounts of elements to drain (must be less than sz)
 * @return the number of elements written
 */
size_t blocking_q_drain_at_least(blocking_q *q, task_ptr *data, size_t sz, size_t min) {

    int counter = 0;

    loop:
    while(q->sz > 0) {
        data[counter++] = __blocking_q_take(q);
    }
    pthread_mutex_lock(&q->lock);

    if (min > counter) {
        while (q->sz < (min - counter)) pthread_cond_wait(&q->cond, &q->lock);
        goto loop;
    }

    return counter;
}

/**
 * Check the first element in the queue. This will allocate storage for a copy
 * of the character. If the allocation fails, this function returns false.
 * @param q the queue
 * @param c pointer to a pointer where an allocated char will be stored
 * @return if there is an element allocated in the pointer
 */
bool blocking_q_peek(blocking_q *q, task **c) {

    if (q->sz == 0) return false;
    c = malloc(sizeof(task_ptr));

    c = &(q->first->data);

}