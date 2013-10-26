#ifndef _DEDUP_UTILS_H_
#define _DEDUP_UTILS_H_

#define ADLER_MOD_VAL 65521

//The Modular offset the largest number below (2^64  -  2*65521) or (2^32 - 2*65521) which is equal to 0. (Mod 65521)
//I.E. ADLER_MODULAR_OFFSET % 65521 == 0
//This avoids wrap-arounds due to the multiplicatin-subtraction that occurs when trying to shift a character out. See adler_shift(...)
//The 64 bit offset avoids wrap-around upto n=72340172838075960, or approx 64 Petabytes in queue
//The 64 bit offset also requires the struct to use (long long)s instead of just (int)s
//#define ADLER_MODULAR_OFFSET 18446744073709369949L

//The 32 bit offset avoids wrap-around upto n=16842494, or approximately 16 Megabytes in queue
#define ADLER_MODULAR_OFFSET 4294836029
#define DEFAULT_ADLER_TABLE_BIT_WIDTH 28

struct adler32 {
	unsigned int aVal; 
	unsigned int bVal;

	int n;
	struct charqueue *charbuff;
};

/* Bitwise table for identifying adler32 entries
	* Each char can contain 3 bits. The remaining 29 are made up between the table addressing
	* and expected hash collisions. (28 bit wide table is ~256MB, but each bit would represent
	* 2^(29-28) = 2^1 = 2 distinct entries.) A 29 bit wide table would have no collisions, but
	* requires 512MB of CONTIGUOUS memory.
	*
	* 0101010101010101010101010101        0        101
	*         <array index>             <N/A>   <bit index>
*/
struct adler32table {
	char *table;
	int addr_shift_size; 
};

void adler_init(struct adler32 *crc, unsigned int size);

void adler_free(struct adler32 *crc);

void adler_table_init(struct adler32table *table, int table_bit_width);

void adler_table_free(struct adler32table *table);

void adler_update(struct adler32 *crc, char nextChar);

void adler_update_multi(struct adler32 *crc, char *inp, int inp_size);

void adler_table_update(struct adler32table *table, struct adler32 *crc);

char adler_table_present(struct adler32table *table, struct adler32 *crc);

#endif
