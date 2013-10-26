#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "charqueue.h"
#include "adlerutils.h"

void adler_init(struct adler32 *crc, unsigned int size) {
	crc->n = 0;
	crc->aVal = 1;
	crc->bVal = 0;
	crc->charbuff = malloc(sizeof(struct charqueue));
	charqueue_init(crc->charbuff, size);
}

void adler_free(struct adler32 *crc) {
	charqueue_free(crc->charbuff);
	free(crc->charbuff);
}

void adler_table_init(struct adler32table *table, int table_bit_width) {
	table->addr_shift_size = (32-table_bit_width);
	debug4("Allocating Adler32 table of %d bytes.\n", 1<<table_bit_width);
	table->table = calloc(1 << table_bit_width, sizeof(char));
}

void adler_table_free(struct adler32table *table) {
	debug4("Freeing Adler32 table of %d bytes.\n", 32-table->addr_shift_size);
	free(table->table);
}

//TODO: This can likely be optimized further if performance is ever an issue here. (And it might be)
//1<<32 == 0x100000000'
void adler_shift(struct adler32 *crc, char prevChar, char nextChar) {
	debug4("Old Char: '%c', New Char: '%c'\n", prevChar, nextChar);
	//The aVal can only decreased by 255 or increased by 255
	crc->aVal = ADLER_MOD_VAL + crc->aVal - prevChar + nextChar;
	
	crc->aVal %= ADLER_MOD_VAL;

	//The bVal can be decreased by n*255 or increased by 2*65521 (aVal + bVal)
	crc->bVal = ADLER_MODULAR_OFFSET + crc->bVal - (crc->n*prevChar) + crc->aVal - 1;
	crc->bVal %= ADLER_MOD_VAL;
}

//Get the upper portion of the adler table address.
unsigned int adler_table_getaddr(struct adler32table *table, struct adler32 *crc) {
	return (( (crc->bVal&0xFFFF)<<16) | (crc->aVal&0xFFFF)) >> table->addr_shift_size;
}

//Retrieve a 1 in the correct offset
unsigned char adler_table_getbit(struct adler32 *crc) {
	return 0x1 << (crc->aVal & 0x7);
}

void adler_update(struct adler32 *crc, char nextChar) {
	if (charqueue_is_full(crc->charbuff)) {
	//if (1) {
		debug4("Updating Adler with new char. Rotating Queue.\n");
		adler_shift(crc, charqueue_pop(crc->charbuff), nextChar);
	} else {
		debug4("Updating Adler with new char. Queue not empty.\n");
		crc->n++;
		crc->aVal += nextChar;
		crc->aVal %= ADLER_MOD_VAL;

		crc->bVal += crc->aVal;
		crc->bVal %= ADLER_MOD_VAL;
	}
	charqueue_push(crc->charbuff, nextChar);
	debug4("New BVal:%0.4x, New AVal:%0.4x\n", crc->bVal, crc->aVal);
}

void adler_update_multi(struct adler32 *crc, char *inp, int inp_size) {
	crc->n += inp_size;
	int i;
	for (i = 0; i < inp_size; i++) {
		adler_update(crc, inp[i]);
	}
}

//Update the given table and mark the given crc as present
void adler_table_update(struct adler32table *table, struct adler32 *crc) {
	debug4("Updating index: %d with bitvalue %u.\n", adler_table_getaddr(table, crc), adler_table_getbit(crc));
	table->table[adler_table_getaddr(table, crc)] |= adler_table_getbit(crc);
}

//Returns whether or not the given crc is in the table
char adler_table_present(struct adler32table *table, struct adler32 *crc) {
	debug4("Checking index: %d with bitvalue %u.\n", adler_table_getaddr(table, crc), adler_table_getbit(crc));
	return table->table[ adler_table_getaddr(table, crc) ] & adler_table_getbit(crc);
}
