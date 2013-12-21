#include "datachunk.h"
#include "openssl/sha.h"

void calculate_hash(struct datachunk *chunk) {
	uint32_t curs;
	SHA512_Init(&chunk->ctx);
	for (curs = 0; curs < chunk->num_chunks; curs++) {
		SHA512_Update(&chunk->ctx, chunk->data[curs], chunk->data_len[curs]);
	}
	SHA512_Final(chunk->hash, &chunk->ctx);

}

