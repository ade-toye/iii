/*
 * uarray2.c
 *
 * Authors: Toye Adebayo & Teny Makuach
 * Date: 2/2/2026
 *
 * Purpose: Implements UArray2, a 2D unboxed array built on top
 *          of Hanson's 1D UArray_T. Provides functions to create
 *          and free a 2D array, query its dimensions and element
 *          size, access an element by (col, row), and traverse
 *          all elements in row-major or column-major order.
 *
 * Key Insight: The 2D array is stored as one flat 1D UArray.
 *          Each (col, row) maps to index = row * width + col.
 *          The map functions change only traversal order, not
 *          how elements are stored.
 */

#include <stdlib.h>
#include "uarray2.h"
#include "uarray.h"
#include "assert.h"
#include "mem.h"

#define T UArray2_T
struct T {
        int width;
        int height;
        int size;
        UArray_T data;
};

/*
 * UArray2_new - see uarray2.h for contract
 */
T UArray2_new(int width, int height, int size)
{
        assert(width > 0);
        assert(height > 0);
        assert(size > 0);

        T uarray2;
        NEW(uarray2);
        uarray2->width  = width;
        uarray2->height = height;
        uarray2->size   = size;
        uarray2->data   = UArray_new(width * height, size);

        return uarray2;
}

/*
 * UArray2_at - see uarray2.h for contract
 */
void *UArray2_at(T uarray2, int col, int row)
{
        assert(uarray2 != NULL);
        assert(col >= 0 && col < uarray2->width);
        assert(row >= 0 && row < uarray2->height);

        int index = row * uarray2->width + col;
        return UArray_at(uarray2->data, index);
}

/*
 * UArray2_height - see uarray2.h for contract
 */
int UArray2_height(T uarray2)
{
        assert(uarray2 != NULL);
        return uarray2->height;
}

/*
 * UArray2_size - see uarray2.h for contract
 */
int UArray2_size(T uarray2)
{
        assert(uarray2 != NULL);
        return uarray2->size;
}

/*
 * UArray2_width - see uarray2.h for contract
 */
int UArray2_width(T uarray2)
{
        assert(uarray2 != NULL);
        return uarray2->width;
}

/*
 * UArray2_free - see uarray2.h for contract
 */
void UArray2_free(T *uarray2)
{
        assert(uarray2 != NULL);
        assert(*uarray2 != NULL);

        UArray_free(&(*uarray2)->data);
        FREE(*uarray2);
}

/*
 * UArray2_map_col_major - see uarray2.h for contract
 */
void UArray2_map_col_major(T uarray2, UArray2_applyfun *apply,
                           void *cl)
{
        assert(uarray2 != NULL);
        assert(apply != NULL);

        for (int col = 0; col < uarray2->width; col++) {
                for (int row = 0; row < uarray2->height; row++) {
                        void *elem = UArray2_at(uarray2, col, row);
                        apply(col, row, uarray2, elem, cl);
                }
        }
}

/*
 * UArray2_map_row_major - see uarray2.h for contract
 */
void UArray2_map_row_major(T uarray2, UArray2_applyfun *apply,
                           void *cl)
{
        assert(uarray2 != NULL);
        assert(apply != NULL);

        for (int row = 0; row < uarray2->height; row++) {
                for (int col = 0; col < uarray2->width; col++) {
                        void *elem = UArray2_at(uarray2, col, row);
                        apply(col, row, uarray2, elem, cl);
                }
        }
}
