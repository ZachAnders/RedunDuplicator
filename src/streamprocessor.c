#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "deduputils.h"
#include "streamprocessor.h"
#include "charqueue.h"
#include "chunkqueue.h"


void init_streamprocessor(struct sproc_cfg *cfg, unsigned int chunksize) {
	cfg->chunksize = chunksize;
	cfg->queue = malloc(sizeof(struct chunkqueue));
	cfg->adler = malloc(sizeof(struct adler32));
	cfg->lookup = malloc(sizeof(struct adler32table));

	queue_init(cfg->queue, DEFAULT_QUEUE_SIZE);
	adler_init(cfg->adler, chunksize);
	adler_table_init(cfg->lookup);

}


void free_streamprocessor(struct sproc_cfg *cfg) {
	queue_free(cfg->queue);
	free(cfg->queue);
	//Nothing to be done for the adler32 or adler32table structs
}

void update_streamprocessor(struct sproc_cfg *cfg, char* datablock, unsigned int datasize) {
	/* 
	 * Cases: 
	 *	- There are bytes left over in the tmp_buf which need to logically prefix datablock
	 *	- There are no bytes in tmp_buf, and we simply construct our sliding windows and begin processing
	 */
	//TODO: Utilize character queue in sproc_cfg
		
}
