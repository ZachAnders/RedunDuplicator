#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <openssl/sha.h>

#ifndef _DATACHUNK_H_
#define _DATACHUNK_H_

#define MAX_DIGEST_SIZE 64
#define MAX_CHUNK_SIZE 64

/* Holds an array of strings. These strings are concatenated during hasing */
struct datachunk {
	SHA512_CTX ctx;
	uint32_t num_chunks;
	const uint8_t *data[MAX_CHUNK_SIZE];
	uint32_t data_len[MAX_CHUNK_SIZE];;
	uint8_t hash[MAX_DIGEST_SIZE];
};

void calculate_hash(struct datachunk *a_chunk);

#endif
