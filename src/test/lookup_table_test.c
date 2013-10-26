#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "adlerutils.h"
#include "charqueue.h"
#include "testdatavals.h"

//Our table may have false positives, but never false negatives
//I.E. For items which ARE in the table, it is ALWAYS correct. (Returns true)
//For items which ARE NOT in the table, it is USUALLY correct. (Returns false, but collisions can return true)
void adler_lookup_table_test(int data_count, int data_offset, int bit_table_width, const char *data, int data_len) {
	debug4("%d %d\n", data_count, bit_table_width);
	struct adler32table testTable;
	struct adler32 adler;
	short *shorts = (short*) data;
	int i;
	assert(data_len >= ((4*data_count)+data_offset)); //4 bytes for a 'fake' adler32 instance
	adler_table_init(&testTable, bit_table_width);
	for (i=data_offset; i < data_count; i++) {
		adler.aVal = shorts[2*i];
		adler.bVal = shorts[2*i + 1];
		adler_table_update(&testTable, &adler);
	}

	for (i=data_offset; i < data_count; i++) {
		adler.aVal = shorts[2*i];
		adler.bVal = shorts[(2*i)+1];
		debug4("Vals: %d %d\n", shorts[2*i], shorts[2*i + 1]);
		assert(adler_table_present(&testTable, &adler));
	}

	adler_table_free(&testTable);
}

void lookup_table_batch_test() {
	int i, j;
	int bit_width, offset, count;
	//Test bit widths 27, 28, and 29.
	//These should occupy 128MB, 256MB, and 512MB
	for (bit_width = 27; bit_width < 30; bit_width++) {
		for (i = 0; i < TEST_DATA_NUM_VALS; i++) {
			debug1("Beginning Lookup Table batch tests for DATA[%d] bit width: %d bits.\n", i, bit_width);
			int dtlen = strlen(TEST_ASCII_DATA[i]);
			adler_lookup_table_test(128, 0, bit_width, TEST_ASCII_DATA[i], dtlen);
			adler_lookup_table_test(256, 0, bit_width, TEST_ASCII_DATA[i], dtlen);
			adler_lookup_table_test(512, 0, bit_width, TEST_ASCII_DATA[i], dtlen);
			for (j = 0; j < TEST_NUM_RAND_TESTS; j++) {
				offset = rand() % (dtlen/2);
				count = rand() % ((dtlen-offset)/4);
				adler_lookup_table_test(count, offset, bit_width, TEST_ASCII_DATA[i], dtlen);
			}
		}
	}
}

int main(int argc, char* argv[]) {
	lookup_table_batch_test();

	debug1("All tests completed successfully.\n");
	argc=0;argv=0;
	return 0;
}
