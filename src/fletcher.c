
#include "fletcher.h"

inline void init(struct fletcher_ctx *ctx) {
	ctx->sum1 = 0;
	ctx->sum2 = 0;
	ctx->count = 0;
}

inline void rotate(struct fletcher_ctx *ctx, uint32_t newval, uint32_t oldval) {
	ctx->sum1 = ctx->sum1 - oldval + newval;
	ctx->sum2 = ctx->sum2 + ctx->sum1 - ctx->count*oldval;
}

inline void roll_in(struct fletcher_ctx *ctx, uint32_t newval) {
	ctx->sum1 += newval;
	ctx->sum2 += ctx->sum1;
	ctx->count++;
}

inline void roll_out(struct fletcher_ctx *ctx, uint32_t oldval) {
	ctx->sum1 -= oldval;
	ctx->sum2 -= ctx->count*oldval;
	ctx->count--;
}
