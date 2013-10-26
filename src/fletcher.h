#ifndef _FLETCHER_H_
#define _FLETCHER_H_

#include <stdint.h>

typedef struct fletcher_ctx {
	uint32_t sum1;
	uint32_t sum2;
	uint32_t count;
} fletcher_ctx;

inline void init(struct fletcher_ctx *ctx);

inline void rotate(struct fletcher_ctx *ctx, uint32_t newval, uint32_t oldval);

inline void roll_in(struct fletcher_ctx *ctx, uint32_t newval);

inline void roll_out(struct fletcher_ctx *ctx, uint32_t oldval);

#endif
