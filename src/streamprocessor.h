
#ifndef _STREAMPROCESSOR_H_
#define _STREAMPROCESSOR_H_


#define DEFAULT_QUEUE_SIZE 2048

struct sproc_cfg {
	struct adler32 *adler;
	struct adler32table *lookup;
	struct chunkqueue *queue;
	unsigned int chunksize;
};


void init_streamprocessor(struct sproc_cfg *cfg, unsigned int chunksize);

void free_streamprocessor(struct sproc_cfg *cfg);

void update_streamprocessor(struct sproc_cfg *cfg, char* datablock, unsigned int datasize);

#endif
