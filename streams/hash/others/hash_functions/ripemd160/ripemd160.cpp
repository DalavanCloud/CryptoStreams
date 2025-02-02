/* ripemd-160.c - an implementation of RIPEMD-160 Hash function
 * based on the original aritcle:
 * H. Dobbertin, A. Bosselaers, B. Preneel, RIPEMD-160: A strengthened version
 * of RIPEMD, Lecture Notes in Computer, 1996, V.1039, pp.71-82
 *
 * Copyright: 2009-2012 Aleksey Kravchenko <rhash.admin@gmail.com>
 *
 * Permission is hereby granted,  free of charge,  to any person  obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction,  including without limitation
 * the rights to  use, copy, modify,  merge, publish, distribute, sublicense,
 * and/or sell copies  of  the Software,  and to permit  persons  to whom the
 * Software is furnished to do so.
 *
 * This program  is  distributed  in  the  hope  that it will be useful,  but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  Use this program  at  your own risk!
 */

#include <cstring>
#include "../whirlpool/byte_order.h"
#include "ripemd160.h"

/**
 * Initialize algorithm context before calculaing hash.
 *
 * @param ctx context to initialize
 */
void rhash_ripemd160_init(ripemd160_ctx *ctx)
{
    ctx->length = 0;

    /* initialize state */
    ctx->hash[0] = 0x67452301;
    ctx->hash[1] = 0xefcdab89;
    ctx->hash[2] = 0x98badcfe;
    ctx->hash[3] = 0x10325476;
    ctx->hash[4] = 0xc3d2e1f0;
}

/* five boolean functions */
#define F1(x, y, z) ((x) ^ (y) ^ (z))
#define F2(x, y, z) ((((y) ^ (z)) & (x)) ^ (z))
#define F3(x, y, z) (((x) | ~(y)) ^ (z))
#define F4(x, y, z) ((((x) ^ (y)) & (z)) ^ (y))
#define F5(x, y, z) ((x) ^ ((y) | ~(z)))

#define RMD_FUNC(FUNC, A, B, C, D, E, X, S, K) \
	(A) += FUNC((B), (C), (D)) + (X) + K; \
	(A) = ROTL32((A), (S)) + (E); \
	(C) = ROTL32((C), 10);

/* steps for the left and right half of algorithm */
#define L1(A, B, C, D, E, X, S) RMD_FUNC(F1, A, B, C, D, E, X, S, 0)
#define L2(A, B, C, D, E, X, S) RMD_FUNC(F2, A, B, C, D, E, X, S, 0x5a827999UL)
#define L3(A, B, C, D, E, X, S) RMD_FUNC(F3, A, B, C, D, E, X, S, 0x6ed9eba1UL)
#define L4(A, B, C, D, E, X, S) RMD_FUNC(F4, A, B, C, D, E, X, S, 0x8f1bbcdcUL)
#define L5(A, B, C, D, E, X, S) RMD_FUNC(F5, A, B, C, D, E, X, S, 0xa953fd4eUL)
#define R1(A, B, C, D, E, X, S) RMD_FUNC(F5, A, B, C, D, E, X, S, 0x50a28be6UL)
#define R2(A, B, C, D, E, X, S) RMD_FUNC(F4, A, B, C, D, E, X, S, 0x5c4dd124UL)
#define R3(A, B, C, D, E, X, S) RMD_FUNC(F3, A, B, C, D, E, X, S, 0x6d703ef3UL)
#define R4(A, B, C, D, E, X, S) RMD_FUNC(F2, A, B, C, D, E, X, S, 0x7a6d76e9UL)
#define R5(A, B, C, D, E, X, S) RMD_FUNC(F1, A, B, C, D, E, X, S, 0)

/**
 * The core transformation. Process a 512-bit block.
 *
 * @param hash algorithm intermediate hash
 * @param X the message block to process
 */
static void rhash_ripemd160_process_block(unsigned *hash, const unsigned* X, unsigned nr)
{
    unsigned A = hash[0],  B = hash[1],  C = hash[2],
        D = hash[3],  E = hash[4];
    unsigned a1, b1, c1, d1, e1;

    /* rounds of the left half */
    L1(A, B, C, D, E, X[ 0], 11); if (nr ==  1) goto finish_l;
    L1(E, A, B, C, D, X[ 1], 14); if (nr ==  2) goto finish_l;
    L1(D, E, A, B, C, X[ 2], 15); if (nr ==  3) goto finish_l;
    L1(C, D, E, A, B, X[ 3], 12); if (nr ==  4) goto finish_l;
    L1(B, C, D, E, A, X[ 4],  5); if (nr ==  5) goto finish_l;
    L1(A, B, C, D, E, X[ 5],  8); if (nr ==  6) goto finish_l;
    L1(E, A, B, C, D, X[ 6],  7); if (nr ==  7) goto finish_l;
    L1(D, E, A, B, C, X[ 7],  9); if (nr ==  8) goto finish_l;
    L1(C, D, E, A, B, X[ 8], 11); if (nr ==  9) goto finish_l;
    L1(B, C, D, E, A, X[ 9], 13); if (nr == 10) goto finish_l;
    L1(A, B, C, D, E, X[10], 14); if (nr == 11) goto finish_l;
    L1(E, A, B, C, D, X[11], 15); if (nr == 12) goto finish_l;
    L1(D, E, A, B, C, X[12],  6); if (nr == 13) goto finish_l;
    L1(C, D, E, A, B, X[13],  7); if (nr == 14) goto finish_l;
    L1(B, C, D, E, A, X[14],  9); if (nr == 15) goto finish_l;
    L1(A, B, C, D, E, X[15],  8); if (nr == 16) goto finish_l;
    L2(E, A, B, C, D, X[ 7],  7); if (nr == 17) goto finish_l;
    L2(D, E, A, B, C, X[ 4],  6); if (nr == 18) goto finish_l;
    L2(C, D, E, A, B, X[13],  8); if (nr == 19) goto finish_l;
    L2(B, C, D, E, A, X[ 1], 13); if (nr == 20) goto finish_l;
    L2(A, B, C, D, E, X[10], 11); if (nr == 21) goto finish_l;
    L2(E, A, B, C, D, X[ 6],  9); if (nr == 22) goto finish_l;
    L2(D, E, A, B, C, X[15],  7); if (nr == 23) goto finish_l;
    L2(C, D, E, A, B, X[ 3], 15); if (nr == 24) goto finish_l;
    L2(B, C, D, E, A, X[12],  7); if (nr == 25) goto finish_l;
    L2(A, B, C, D, E, X[ 0], 12); if (nr == 26) goto finish_l;
    L2(E, A, B, C, D, X[ 9], 15); if (nr == 27) goto finish_l;
    L2(D, E, A, B, C, X[ 5],  9); if (nr == 28) goto finish_l;
    L2(C, D, E, A, B, X[ 2], 11); if (nr == 29) goto finish_l;
    L2(B, C, D, E, A, X[14],  7); if (nr == 30) goto finish_l;
    L2(A, B, C, D, E, X[11], 13); if (nr == 31) goto finish_l;
    L2(E, A, B, C, D, X[ 8], 12); if (nr == 32) goto finish_l;
    L3(D, E, A, B, C, X[ 3], 11); if (nr == 33) goto finish_l;
    L3(C, D, E, A, B, X[10], 13); if (nr == 34) goto finish_l;
    L3(B, C, D, E, A, X[14],  6); if (nr == 35) goto finish_l;
    L3(A, B, C, D, E, X[ 4],  7); if (nr == 36) goto finish_l;
    L3(E, A, B, C, D, X[ 9], 14); if (nr == 37) goto finish_l;
    L3(D, E, A, B, C, X[15],  9); if (nr == 38) goto finish_l;
    L3(C, D, E, A, B, X[ 8], 13); if (nr == 39) goto finish_l;
    L3(B, C, D, E, A, X[ 1], 15); if (nr == 40) goto finish_l;
    L3(A, B, C, D, E, X[ 2], 14); if (nr == 41) goto finish_l;
    L3(E, A, B, C, D, X[ 7],  8); if (nr == 42) goto finish_l;
    L3(D, E, A, B, C, X[ 0], 13); if (nr == 43) goto finish_l;
    L3(C, D, E, A, B, X[ 6],  6); if (nr == 44) goto finish_l;
    L3(B, C, D, E, A, X[13],  5); if (nr == 45) goto finish_l;
    L3(A, B, C, D, E, X[11], 12); if (nr == 46) goto finish_l;
    L3(E, A, B, C, D, X[ 5],  7); if (nr == 47) goto finish_l;
    L3(D, E, A, B, C, X[12],  5); if (nr == 48) goto finish_l;
    L4(C, D, E, A, B, X[ 1], 11); if (nr == 49) goto finish_l;
    L4(B, C, D, E, A, X[ 9], 12); if (nr == 50) goto finish_l;
    L4(A, B, C, D, E, X[11], 14); if (nr == 51) goto finish_l;
    L4(E, A, B, C, D, X[10], 15); if (nr == 52) goto finish_l;
    L4(D, E, A, B, C, X[ 0], 14); if (nr == 53) goto finish_l;
    L4(C, D, E, A, B, X[ 8], 15); if (nr == 54) goto finish_l;
    L4(B, C, D, E, A, X[12],  9); if (nr == 55) goto finish_l;
    L4(A, B, C, D, E, X[ 4],  8); if (nr == 56) goto finish_l;
    L4(E, A, B, C, D, X[13],  9); if (nr == 57) goto finish_l;
    L4(D, E, A, B, C, X[ 3], 14); if (nr == 58) goto finish_l;
    L4(C, D, E, A, B, X[ 7],  5); if (nr == 59) goto finish_l;
    L4(B, C, D, E, A, X[15],  6); if (nr == 60) goto finish_l;
    L4(A, B, C, D, E, X[14],  8); if (nr == 61) goto finish_l;
    L4(E, A, B, C, D, X[ 5],  6); if (nr == 62) goto finish_l;
    L4(D, E, A, B, C, X[ 6],  5); if (nr == 63) goto finish_l;
    L4(C, D, E, A, B, X[ 2], 12); if (nr == 64) goto finish_l;
    L5(B, C, D, E, A, X[ 4],  9); if (nr == 65) goto finish_l;
    L5(A, B, C, D, E, X[ 0], 15); if (nr == 66) goto finish_l;
    L5(E, A, B, C, D, X[ 5],  5); if (nr == 67) goto finish_l;
    L5(D, E, A, B, C, X[ 9], 11); if (nr == 68) goto finish_l;
    L5(C, D, E, A, B, X[ 7],  6); if (nr == 69) goto finish_l;
    L5(B, C, D, E, A, X[12],  8); if (nr == 70) goto finish_l;
    L5(A, B, C, D, E, X[ 2], 13); if (nr == 71) goto finish_l;
    L5(E, A, B, C, D, X[10], 12); if (nr == 72) goto finish_l;
    L5(D, E, A, B, C, X[14],  5); if (nr == 73) goto finish_l;
    L5(C, D, E, A, B, X[ 1], 12); if (nr == 74) goto finish_l;
    L5(B, C, D, E, A, X[ 3], 13); if (nr == 75) goto finish_l;
    L5(A, B, C, D, E, X[ 8], 14); if (nr == 76) goto finish_l;
    L5(E, A, B, C, D, X[11], 11); if (nr == 77) goto finish_l;
    L5(D, E, A, B, C, X[ 6],  8); if (nr == 78) goto finish_l;
    L5(C, D, E, A, B, X[15],  5); if (nr == 79) goto finish_l;
    L5(B, C, D, E, A, X[13],  6); if (nr == 80) goto finish_l;

finish_l:
    a1 = A, b1 = B, c1 = C, d1 = D, e1 = E;
    A = hash[0],  B = hash[1],  C = hash[2],
    D = hash[3],  E = hash[4];

    /* rounds of the right half */
    R1(A, B, C, D, E, X[ 5],  8); if (nr ==  1) goto finish_r;
    R1(E, A, B, C, D, X[14],  9); if (nr ==  2) goto finish_r;
    R1(D, E, A, B, C, X[ 7],  9); if (nr ==  3) goto finish_r;
    R1(C, D, E, A, B, X[ 0], 11); if (nr ==  4) goto finish_r;
    R1(B, C, D, E, A, X[ 9], 13); if (nr ==  5) goto finish_r;
    R1(A, B, C, D, E, X[ 2], 15); if (nr ==  6) goto finish_r;
    R1(E, A, B, C, D, X[11], 15); if (nr ==  7) goto finish_r;
    R1(D, E, A, B, C, X[ 4],  5); if (nr ==  8) goto finish_r;
    R1(C, D, E, A, B, X[13],  7); if (nr ==  9) goto finish_r;
    R1(B, C, D, E, A, X[ 6],  7); if (nr == 10) goto finish_r;
    R1(A, B, C, D, E, X[15],  8); if (nr == 11) goto finish_r;
    R1(E, A, B, C, D, X[ 8], 11); if (nr == 12) goto finish_r;
    R1(D, E, A, B, C, X[ 1], 14); if (nr == 13) goto finish_r;
    R1(C, D, E, A, B, X[10], 14); if (nr == 14) goto finish_r;
    R1(B, C, D, E, A, X[ 3], 12); if (nr == 15) goto finish_r;
    R1(A, B, C, D, E, X[12],  6); if (nr == 16) goto finish_r;
    R2(E, A, B, C, D, X[ 6],  9); if (nr == 17) goto finish_r;
    R2(D, E, A, B, C, X[11], 13); if (nr == 18) goto finish_r;
    R2(C, D, E, A, B, X[ 3], 15); if (nr == 19) goto finish_r;
    R2(B, C, D, E, A, X[ 7],  7); if (nr == 20) goto finish_r;
    R2(A, B, C, D, E, X[ 0], 12); if (nr == 21) goto finish_r;
    R2(E, A, B, C, D, X[13],  8); if (nr == 22) goto finish_r;
    R2(D, E, A, B, C, X[ 5],  9); if (nr == 23) goto finish_r;
    R2(C, D, E, A, B, X[10], 11); if (nr == 24) goto finish_r;
    R2(B, C, D, E, A, X[14],  7); if (nr == 25) goto finish_r;
    R2(A, B, C, D, E, X[15],  7); if (nr == 26) goto finish_r;
    R2(E, A, B, C, D, X[ 8], 12); if (nr == 27) goto finish_r;
    R2(D, E, A, B, C, X[12],  7); if (nr == 28) goto finish_r;
    R2(C, D, E, A, B, X[ 4],  6); if (nr == 29) goto finish_r;
    R2(B, C, D, E, A, X[ 9], 15); if (nr == 30) goto finish_r;
    R2(A, B, C, D, E, X[ 1], 13); if (nr == 31) goto finish_r;
    R2(E, A, B, C, D, X[ 2], 11); if (nr == 32) goto finish_r;
    R3(D, E, A, B, C, X[15],  9); if (nr == 33) goto finish_r;
    R3(C, D, E, A, B, X[ 5],  7); if (nr == 34) goto finish_r;
    R3(B, C, D, E, A, X[ 1], 15); if (nr == 35) goto finish_r;
    R3(A, B, C, D, E, X[ 3], 11); if (nr == 36) goto finish_r;
    R3(E, A, B, C, D, X[ 7],  8); if (nr == 37) goto finish_r;
    R3(D, E, A, B, C, X[14],  6); if (nr == 38) goto finish_r;
    R3(C, D, E, A, B, X[ 6],  6); if (nr == 39) goto finish_r;
    R3(B, C, D, E, A, X[ 9], 14); if (nr == 40) goto finish_r;
    R3(A, B, C, D, E, X[11], 12); if (nr == 41) goto finish_r;
    R3(E, A, B, C, D, X[ 8], 13); if (nr == 42) goto finish_r;
    R3(D, E, A, B, C, X[12],  5); if (nr == 43) goto finish_r;
    R3(C, D, E, A, B, X[ 2], 14); if (nr == 44) goto finish_r;
    R3(B, C, D, E, A, X[10], 13); if (nr == 45) goto finish_r;
    R3(A, B, C, D, E, X[ 0], 13); if (nr == 46) goto finish_r;
    R3(E, A, B, C, D, X[ 4],  7); if (nr == 47) goto finish_r;
    R3(D, E, A, B, C, X[13],  5); if (nr == 48) goto finish_r;
    R4(C, D, E, A, B, X[ 8], 15); if (nr == 49) goto finish_r;
    R4(B, C, D, E, A, X[ 6],  5); if (nr == 50) goto finish_r;
    R4(A, B, C, D, E, X[ 4],  8); if (nr == 51) goto finish_r;
    R4(E, A, B, C, D, X[ 1], 11); if (nr == 52) goto finish_r;
    R4(D, E, A, B, C, X[ 3], 14); if (nr == 53) goto finish_r;
    R4(C, D, E, A, B, X[11], 14); if (nr == 54) goto finish_r;
    R4(B, C, D, E, A, X[15],  6); if (nr == 55) goto finish_r;
    R4(A, B, C, D, E, X[ 0], 14); if (nr == 56) goto finish_r;
    R4(E, A, B, C, D, X[ 5],  6); if (nr == 57) goto finish_r;
    R4(D, E, A, B, C, X[12],  9); if (nr == 58) goto finish_r;
    R4(C, D, E, A, B, X[ 2], 12); if (nr == 59) goto finish_r;
    R4(B, C, D, E, A, X[13],  9); if (nr == 60) goto finish_r;
    R4(A, B, C, D, E, X[ 9], 12); if (nr == 61) goto finish_r;
    R4(E, A, B, C, D, X[ 7],  5); if (nr == 62) goto finish_r;
    R4(D, E, A, B, C, X[10], 15); if (nr == 63) goto finish_r;
    R4(C, D, E, A, B, X[14],  8); if (nr == 64) goto finish_r;
    R5(B, C, D, E, A, X[12] ,  8); if (nr == 65) goto finish_r;
    R5(A, B, C, D, E, X[15] ,  5); if (nr == 66) goto finish_r;
    R5(E, A, B, C, D, X[10] , 12); if (nr == 67) goto finish_r;
    R5(D, E, A, B, C, X[ 4] ,  9); if (nr == 68) goto finish_r;
    R5(C, D, E, A, B, X[ 1] , 12); if (nr == 69) goto finish_r;
    R5(B, C, D, E, A, X[ 5] ,  5); if (nr == 70) goto finish_r;
    R5(A, B, C, D, E, X[ 8] , 14); if (nr == 71) goto finish_r;
    R5(E, A, B, C, D, X[ 7] ,  6); if (nr == 72) goto finish_r;
    R5(D, E, A, B, C, X[ 6] ,  8); if (nr == 73) goto finish_r;
    R5(C, D, E, A, B, X[ 2] , 13); if (nr == 74) goto finish_r;
    R5(B, C, D, E, A, X[13] ,  6); if (nr == 75) goto finish_r;
    R5(A, B, C, D, E, X[14] ,  5); if (nr == 76) goto finish_r;
    R5(E, A, B, C, D, X[ 0] , 15); if (nr == 77) goto finish_r;
    R5(D, E, A, B, C, X[ 3] , 13); if (nr == 78) goto finish_r;
    R5(C, D, E, A, B, X[ 9] , 11); if (nr == 79) goto finish_r;
    R5(B, C, D, E, A, X[11] , 11); if (nr == 80) goto finish_r;

finish_r:
    /* update intermediate hash */
    D += c1 + hash[1];
    hash[1] = hash[2] + d1 + E;
    hash[2] = hash[3] + e1 + A;
    hash[3] = hash[4] + a1 + B;
    hash[4] = hash[0] + b1 + C;
    hash[0] = D;
}

/**
 * Calculate message hash.
 * Can be called repeatedly with chunks of the message to be hashed.
 *
 * @param ctx the algorithm context containing current hashing state
 * @param msg message chunk
 * @param size length of the message chunk
 */
void rhash_ripemd160_update(ripemd160_ctx *ctx, const unsigned char* msg, size_t size, unsigned nr)
{
    unsigned index = (unsigned)ctx->length & 63;
    ctx->length += size;

    /* fill partial block */
    if (index) {
        unsigned left = ripemd160_block_size - index;
        le32_copy((char*)ctx->message, index, msg, (size < left ? size : left));
        if (size < left) return;

        /* process partial block */
        rhash_ripemd160_process_block(ctx->hash, (unsigned*)ctx->message, nr);
        msg += left;
        size -= left;
    }
    while (size >= ripemd160_block_size) {
        unsigned* aligned_message_block;
        if (IS_LITTLE_ENDIAN && IS_ALIGNED_32(msg)) {
            /* the most common case is processing of an already aligned message
            on little-endian CPU without copying it */
            aligned_message_block = (unsigned*)msg;
        } else {
            le32_copy(ctx->message, 0, msg, ripemd160_block_size);
            aligned_message_block = ctx->message;
        }

        rhash_ripemd160_process_block(ctx->hash, aligned_message_block, nr);
        msg += ripemd160_block_size;
        size -= ripemd160_block_size;
    }
    if (size) {
        /* save leftovers */
        le32_copy(ctx->message, 0, msg, size);
    }
}

/**
 * Store calculated hash into the given array.
 *
 * @param ctx the algorithm context containing current hashing state
 * @param result calculated hash in binary form
 */
void rhash_ripemd160_final(ripemd160_ctx *ctx, unsigned char result[20], unsigned nr)
{
    unsigned index = ((unsigned)ctx->length & 63) >> 2;
    unsigned shift = ((unsigned)ctx->length & 3) * 8;

    /* pad message and run for last block */

    /* append the byte 0x80 to the message */
    ctx->message[index]   &= ~(0xFFFFFFFFu << shift);
    ctx->message[index++] ^= 0x80u << shift;

    /* if no room left in the message to store 64-bit message length */
    if (index > 14) {
        /* then fill the rest with zeros and process it */
        while (index < 16) {
            ctx->message[index++] = 0;
        }
        rhash_ripemd160_process_block(ctx->hash, ctx->message, nr);
        index = 0;
    }
    while (index < 14) {
        ctx->message[index++] = 0;
    }
    ctx->message[14] = (unsigned)(ctx->length << 3);
    ctx->message[15] = (unsigned)(ctx->length >> 29);
    rhash_ripemd160_process_block(ctx->hash, ctx->message, nr);

    le32_copy(result, 0, &ctx->hash, 20);
}
