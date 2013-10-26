
#ifndef _CHARACTERQUEUE_H_
#define _CHARACTERQUEUE_H_
#include <stdio.h>
#include "debugprint.h"

//Very compact queue impl
struct charqueue {
	char* chars;
	unsigned int head;
	unsigned int tail;
	unsigned int len;
	unsigned int size;
};

void charqueue_init(struct charqueue *queue, unsigned int size);

char *charqueue_get_queue_as_chars(char *out, struct charqueue *queue);

void charqueue_push(struct charqueue *queue, char data);

void charqueue_push_many(struct charqueue *queue, const char* data, unsigned int datalen);

char charqueue_pop(struct charqueue *queue);

void charqueue_free(struct charqueue *queue);

int charqueue_is_full(struct charqueue *queue);

void charqueue_debug_print_impl(struct charqueue *queue);

#ifdef DEBUG4
#define charqueue_debug_print charqueue_debug_print_impl
#else
#define charqueue_debug_print(...) 
#endif

#endif
