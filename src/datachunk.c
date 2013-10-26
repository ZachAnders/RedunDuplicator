#include "datachunk.h"
#include "openssl/sha.h"


void calculate_hash(struct datachunk *a_chunk) {
	SHA512((unsigned char *) a_chunk->data, a_chunk->data_len, (unsigned char *) a_chunk->hash);
}

