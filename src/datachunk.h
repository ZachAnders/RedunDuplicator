#include <stdio.h>
#include <unistd.h>
#include <openssl/sha.h>

#ifndef _DATACHUNK_H_
#define _DATACHUNK_H_

#define MAX_DIGEST_SIZE 64

struct datachunk {
	short data_len;
	const char *data;
	const char hash[MAX_DIGEST_SIZE];
};

void calculate_hash(struct datachunk *a_chunk);

#endif
