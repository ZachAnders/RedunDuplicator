#include <stdint.h>
#include <assert.h>

#include "debugprint.h"
#include "gf_complete.h"
//#include "gf_general.h"
#include "gf_rand.h"

#define FIELD_SIZE 32
#define NUM_CHECKSUMS 8


//Should create a vmond row for the given row number
//row_base specifies what base is being exponentiated. I.E. 4^0, 4^1, 4^2, 4^3, etc...
void createvmond(gf_t *gfield, uint32_t *matrix, uint32_t num_rows, uint32_t row_base) {
	uint32_t i;
	matrix[0] = 0x00000001;
	for (i = 1; i < num_rows; i++) {
		matrix[i] = gfield->multiply.w32(gfield, row_base, matrix[i-1]);
	}
}

void updatechecksum(gf_t *gfield, uint32_t *chksum, uint32_t *vmond_row, uint32_t num_items, uint32_t data) {
	gfield->multiply_region.w32(gfield, vmond_row, chksum, data, num_items*sizeof(uint32_t), 1);

}

void printarray(uint32_t *matrix, uint32_t size) {
	uint32_t i;
	for (i=0; i < size; i++) {
		if (!(i%10))
			printf("\n");
		printf("%08x ", matrix[i]);
	}
	printf("\n");
}

int main(int argc, char **argv) {
	gf_t *gfield_context = malloc(sizeof(gf_t));
	uint32_t *vmond_row = malloc(NUM_CHECKSUMS*sizeof(uint32_t));
	uint32_t *chksums = calloc(NUM_CHECKSUMS, sizeof(uint32_t));
	uint32_t data = 0x41414141;

	debug4("Initializing gf.\n");
	gf_init_easy(gfield_context, FIELD_SIZE);

	for (int i = 0; i < 32; i++) {
		printarray(chksums, NUM_CHECKSUMS);
		createvmond(gfield_context, vmond_row, NUM_CHECKSUMS, i+1);
		updatechecksum(gfield_context, chksums, vmond_row, NUM_CHECKSUMS, data);
		//stepvmond(gfield_context, vmond_matrix, FIELD_SIZE);
	}
	printarray(chksums, NUM_CHECKSUMS);

	//free(gfield_context);
	argc=0; argv=0;
}

void stepvmond(gf_t *gfield, uint32_t *matrix, uint32_t w) {
	uint32_t i;
	assert(w==32);
	for (i=1; i < NUM_CHECKSUMS+1; i++) {
		matrix[i-1] = gfield->multiply.w32(gfield, i, matrix[i-1]);
	}
}

void initvmond(uint32_t *matrix, uint32_t w) {
	uint32_t i;
	for (i=0; i < NUM_CHECKSUMS; i++) {
		matrix[i] = 0;
	}
	w=0;
}
