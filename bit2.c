/*
 * bit2.c
 *
 * Authors: Toye Adebayo & Teny Makuach
 * Date: 2/2/2026
 *
 * Purpose: Implements Bit2, a 2D bitmap built on top of
 *          Hanson's 1D Bit_T. Provides functions to create and
 *          free a bitmap, query its dimensions, get and put
 *          individual bits, and traverse all bits in row-major
 *          or column-major order.
 *
 * Key Insight: The 2D bitmap is stored as one flat Bit_T
 *          vector. Each (col, row) maps to the 1D index
 *          row * width + col.
 */

#include <stdlib.h>
#include "bit2.h"
#include "bit.h"
#include "mem.h"
#include "assert.h"

#define T Bit2_T
struct T {
        int width;
        int height;
        Bit_T data;
};

/*
 * Bit2_new - see bit2.h for contract
 */
T Bit2_new(int width, int height)
{
        assert(width > 0);
        assert(height > 0);

        T bit2;
        NEW(bit2);
        bit2->width  = width;
        bit2->height = height;
        bit2->data   = Bit_new(width * height);

        return bit2;
}

/*
 * Bit2_get - see bit2.h for contract
 */
int Bit2_get(T bit2, int col, int row)
{
        assert(bit2 != NULL);
        assert(col >= 0 && col < bit2->width);
        assert(row >= 0 && row < bit2->height);

        int index = row * bit2->width + col;
        return Bit_get(bit2->data, index);
}

/*
 * Bit2_put - see bit2.h for contract
 */
int Bit2_put(T bit2, int col, int row, int value)
{
        assert(bit2 != NULL);
        assert(col >= 0 && col < bit2->width);
        assert(row >= 0 && row < bit2->height);
        assert(value == 0 || value == 1);

        int index = row * bit2->width + col;
        return Bit_put(bit2->data, index, value);
}

/*
 * Bit2_width - see bit2.h for contract
 */
int Bit2_width(T bit2)
{
        assert(bit2 != NULL);
        return bit2->width;
}

/*
 * Bit2_height - see bit2.h for contract
 */
int Bit2_height(T bit2)
{
        assert(bit2 != NULL);
        return bit2->height;
}

/*
 * Bit2_map_col_major - see bit2.h for contract
 */
void Bit2_map_col_major(T bit2, Bit2_applyfun *apply, void *cl)
{
        assert(bit2 != NULL);
        assert(apply != NULL);

        for (int col = 0; col < bit2->width; col++) {
                for (int row = 0; row < bit2->height; row++) {
                        int elem = Bit2_get(bit2, col, row);
                        apply(col, row, bit2, elem, cl);
                }
        }
}

/*
 * Bit2_map_row_major - see bit2.h for contract
 */
void Bit2_map_row_major(T bit2, Bit2_applyfun *apply, void *cl)
{
        assert(bit2 != NULL);
        assert(apply != NULL);

        for (int row = 0; row < bit2->height; row++) {
                for (int col = 0; col < bit2->width; col++) {
                        int elem = Bit2_get(bit2, col, row);
                        apply(col, row, bit2, elem, cl);
                }
        }
}

/*
 * Bit2_free - see bit2.h for contract
 */
void Bit2_free(T *bit2)
{
        assert(bit2 != NULL);
        assert(*bit2 != NULL);

        Bit_free(&((*bit2)->data));
        FREE(*bit2);
}
