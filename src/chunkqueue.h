
#ifndef _CHUNK_QUEUE_H
#define _CHUNK_QUEUE_H

//Synchronized blocking queue for passing chunks between threads via shared memory

struct chunkqueue {
	struct datachunk **data;
	unsigned int head;
	unsigned int tail;
	unsigned int size;
	sem_t *write_sem, *read_sem;
	pthread_mutex_t* modify_lock;
};

void queue_init(struct chunkqueue *queue, unsigned int qsize);

void push_chunk(struct chunkqueue *queue, struct datachunk *data);

struct datachunk* pop_chunk(struct chunkqueue *queue);

void queue_free(struct chunkqueue *queue);

#endif

