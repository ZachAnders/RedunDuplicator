#include <stdint.h>
#include <assert.h>
#include "debugprint.h"
#include "gf_general.h"

#define ITEM(matr, y, x, dimension) matr[y*dimension + x]


//Simple gaussian elimination implementation of matrix inversion on square matrices
//Matrices are stored in row major order
//Since the matrix should be square, both dimensions should be the same. (So we only need a single dimension)
void invert_sq(uint32_t *matrix, uint32_t *dest, uint32_t dimension) {
	assert(matrix);
	uint32_t **augmented = malloc(dimension*dimension*sizeof(uint32_t)*2);
}

void copy_matrix(uint32_t *src, uint32_t *dest, uint32_t srcdim, uint32_t destwidth) {
	uint32_t i, j;
	for (i = 0; i < srcdim; i++) {
		for (j = 0; j < srcdim; j++) {
			ITEM(dest, i, j, destwidth) = ITEM(src, i, j, srcdim);
		}
	}
}

void augment_sq(uint32_t *matrix, uint32_t *dest, uint32_t srcdim, uint32_t destwidth) {
	debug4("Copying source matrix into augmented matrix\n");
	copy_matrix(matrix, dest, srcdim, destwidth);
	
	uint32_t offset = srcdim;
	uint32_t i;
	debug4("Filling identity matrix in augmented matrix.");
	for (i = 0; i < srcdim; i++) {
		ITEM(dest, i, i+srcdim, destwidth) = 0x01;
	}
}
