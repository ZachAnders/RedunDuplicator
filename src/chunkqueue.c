#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include "datachunk.h"
#include "chunkqueue.h"


void queue_init(struct chunkqueue *queue, unsigned int qsize) {
	if (queue->data)
		free(queue->data);
	queue->size = qsize;
	queue->head = 0;
	queue->tail = 0;

	queue->data = malloc(sizeof(struct datachunk)*qsize);
	queue->write_sem = malloc(sizeof(sem_t));
	queue->read_sem = malloc(sizeof(sem_t));
	queue->modify_lock = malloc(sizeof(pthread_mutex_t));
	
	sem_init(queue->write_sem, 0, queue->size);
	sem_init(queue->read_sem, 0, 0);
	pthread_mutex_init(queue->modify_lock, NULL);
}

void push_chunk(struct chunkqueue *queue, struct datachunk *data) {
	while (sem_wait(queue->write_sem)); //sem_wait can be interrupted, so we loop until it returns success. (return code 0)

	pthread_mutex_lock(queue->modify_lock);

	sem_post(queue->read_sem);
	queue->data[queue->head] = data;
	queue->head++;
	queue->head %= queue->size;

	pthread_mutex_unlock(queue->modify_lock);
}

struct datachunk* pop_chunk(struct chunkqueue *queue) {
	struct datachunk *ret;
	while (sem_wait(queue->read_sem)); //sem_wait can be interrupted, so we loop until it returns success. (return code 0)

	pthread_mutex_lock(queue->modify_lock);

	sem_post(queue->write_sem);
	ret = queue->data[queue->tail];
	queue->tail++;
	queue->tail %= queue->size;

	pthread_mutex_unlock(queue->modify_lock);
	return ret;
}
	

void queue_free(struct chunkqueue *queue) {

	if (queue->head != queue->tail) 
		fprintf(stderr, "WARNING: The queue is being freed but it appears to still contain items. This is likely a memory leak!");
	
	sem_destroy(queue->read_sem);
	sem_destroy(queue->write_sem);
	pthread_mutex_destroy(queue->modify_lock);

	free(queue->data);
	free(queue->read_sem);
	free(queue->write_sem);
	free(queue->modify_lock);
}
