#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "adlerutils.h"
#include "charqueue.h"
#include "testdatavals.h"

//Naive implementation based off the wikipedia article
//http://en.wikipedia.org/wiki/Adler-32
void verify_adler32(char *inp, struct adler32 *crc) {
	int MOD = 65521;
	unsigned int a=1, b=0;
	char *curs = inp;
	while (*curs != '\0') {
		a = (a + *curs) % MOD;
		b = (b + a) % MOD;
		curs++;
	}
	debug4("Verifing Adler32: %.4hx%.4hx against properly calculated %.4hx%.4hx\n", (short) crc->bVal, (short) crc->aVal, (short) b, (short) a);
	assert(a == crc->aVal);
	assert(b == crc->bVal);
}

void simple_sliding_window_test(int chunksize, const char *data, int data_len) {
	struct adler32 adler;
	const char *tmp = data;
	int i, counter = 0;
	const char *end_ptr = data+data_len;
	adler_init(&adler, chunksize);

	for (i=0; i < chunksize; i++) {
		adler_update(&adler, *tmp);
		tmp++;
	}

	char *arr = calloc(chunksize+2, sizeof(char));

	while (tmp != end_ptr) {
		//TODO: Okay, bad idea mallocing inside charqueue. Okay for now since it's just a Test...
		charqueue_get_queue_as_chars(arr, adler.charbuff);
		verify_adler32(arr, &adler);
		adler_update(&adler, *tmp);
		tmp++;
		counter++;
	}

	charqueue_get_queue_as_chars(arr, adler.charbuff);
	verify_adler32(arr, &adler);
	counter++;

	free(arr);
	adler_free(&adler);
	debug1("Verified %d Adler32 hashes of chunksize %d during sliding window test.\n", counter, chunksize);
}

void sliding_window_batch_test() {
	int i, j, count;
	for (i=0; i < TEST_DATA_NUM_VALS ; i++) {
		debug1("Beginning Sliding Window Batch Test with Data set [%d].\n", i);
		int dtlen = strlen(TEST_ASCII_DATA[i]);
		simple_sliding_window_test(256, TEST_ASCII_DATA[i], dtlen);
		simple_sliding_window_test(1024, TEST_ASCII_DATA[i], dtlen);
		simple_sliding_window_test(2048, TEST_ASCII_DATA[i], dtlen);
		simple_sliding_window_test(617, TEST_ASCII_DATA[i], dtlen);
		simple_sliding_window_test(1049, TEST_ASCII_DATA[i], dtlen);
		simple_sliding_window_test(2293, TEST_ASCII_DATA[i], dtlen);
		simple_sliding_window_test(7, TEST_ASCII_DATA[i], dtlen);
		simple_sliding_window_test(1789, TEST_ASCII_DATA[i], dtlen);
		for (j = 0; j < TEST_NUM_RAND_TESTS; j++) {
			count = (rand()%(dtlen-1))+1;
			simple_sliding_window_test(count, TEST_ASCII_DATA[i], dtlen);
		}
	}
}

int main(int argc, char* argv[]) {
	sliding_window_batch_test();

	debug1("All tests completed successfully.\n");
	argc=0;argv=0;
	return 0;
}
