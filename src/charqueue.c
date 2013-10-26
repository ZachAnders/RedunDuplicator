#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "charqueue.h"
#include "debugprint.h"

void charqueue_init(struct charqueue *queue, unsigned int size) {
	debug3("Initing charqueue\n");
	queue->chars = malloc(sizeof(char)*(size)); 
	queue->head = 0;
	queue->tail = 0;
	queue->len = 0;
	queue->size = size;
}

char *charqueue_get_queue_as_chars(char *out, struct charqueue *queue) {
	int cursor = queue->tail;
	unsigned int tmp;
	char *ret = out;
	char *retcursor = ret;
	for (tmp = 0; tmp < queue->len; tmp++) {
		(*retcursor) = queue->chars[cursor];
		cursor = (cursor+1)%queue->size;
		retcursor++;
	}
	out[queue->len] = '\0';
	return ret;
}
	

void charqueue_debug_print_impl(struct charqueue *queue) {
	char *tmp = malloc(queue->size+1);
	charqueue_get_queue_as_chars(tmp, queue);
	debug4("New Queue State: '%s'\n", tmp);
	free(tmp);
}

void charqueue_push(struct charqueue *queue, char data) {
	debug4("Pushing char '%c'\n", data);
	//assert(!charqueue_is_full(queue));
	queue->chars[queue->head] = data;
	queue->head++;
	queue->head %= queue->size;
	queue->len++;
	charqueue_debug_print(queue);
	debug4("New Head: %d, Tail: %d\n", queue->head, queue->tail);
}

void charqueue_push_many(struct charqueue *queue, const char* data, unsigned int datalen) {
	unsigned int tmplen = queue->size - queue->head;
	queue->len += datalen;
	assert(queue->len <= queue->size);
	if (datalen > (queue->size - queue->head)) {
		//If tail is not less than head, it has not yet wrapped. This means our array does not have enough space. And that should not happen.
		memcpy(queue->chars+queue->head, data, tmplen);
		queue->head = 0;
		data += tmplen;
		datalen -= tmplen;
	}
	tmplen = queue->tail - queue->head - 1; //-1 because one char is reserved
	memcpy(queue->chars+queue->head, data, datalen);
	queue->head += datalen;
}

char charqueue_pop(struct charqueue *queue) {
	char ret;
	//TODO: This check is superfluous when we are sure the queue is being used *properly*. Could use debug macro guards around this
	//assert(queue->len > 0);
	debug4("Popping char '%c'\n", queue->chars[queue->tail]);
	ret = queue->chars[queue->tail];
	queue->tail++;
	queue->tail %= queue->size;
	queue->len--;
	//charqueue_debug_print(queue);
	debug4("New Head: %d, Tail: %d\n", queue->head, queue->tail);
	return ret;
}

void charqueue_free(struct charqueue *queue) {
	debug3("Freeing charqueue\n");
	free(queue->chars);
}

int charqueue_is_full(struct charqueue *queue) {
	debug4("%d %d\n", queue->len, queue->size);
	return queue->len == queue->size;
}
