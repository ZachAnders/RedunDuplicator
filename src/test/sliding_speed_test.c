#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include "adlerutils.h"
#include "charqueue.h"
#include "testdatavals.h"

void sliding_window_speed_test(int chunksize, const char *data, int data_len, long long num_bytes) {
	struct adler32 adler;
	long long counter = 0;
	adler_init(&adler, chunksize);
	clock_t end, begin = clock();

	while (counter < num_bytes) {
		adler_update(&adler, data[counter%data_len]);
		counter++;
	}

	end = clock();
	debug1("Finished Sliding window speed test with %lld bytes in %f seconds, using %d byte window.\n", num_bytes, ((float) end-begin)/CLOCKS_PER_SEC, chunksize);

	adler_free(&adler);
}

void sliding_speed_batch_test() {
	int i;
	for (i=0; i < TEST_DATA_NUM_VALS; i++) {
		debug1("Beginning sliding window speed test with data set [%d].\n", i);
		int dtlen = strlen(TEST_ASCII_DATA[i]);
		sliding_window_speed_test(1024, TEST_ASCII_DATA[i], dtlen, 536870912L);
	}
}

int main(int argc, char* argv[]) {
	sliding_speed_batch_test();

	debug1("All tests completed successfully.\n");
	argc=0;argv=0;
	return 0;
}
