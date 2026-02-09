/*
 * bit2.h
 *
 * Authors: Toye Adebayo & Teny Makuach
 * Date: 1/31/2026
 *
 * Purpose: Defines the public interface for Bit2, a 2D bitmap
 *          whose elements are bits (0 or 1). Supports creating
 *          and freeing a width-by-height grid of bits, reading
 *          and writing individual bits using (col, row)
 *          coordinates, and traversing all bits in either
 *          row-major or column-major order.
 *
 * Key Insight: Bit2 saves space by storing pixels as packed
 *          bits via Hanson's Bit_T. Because a single bit has
 *          no address, the interface uses put/get rather than
 *          an 'at' function that returns a pointer.
 */

#ifndef BIT2_INCLUDED
#define BIT2_INCLUDED

#define T Bit2_T
typedef struct T *T;

/*
 * Bit2_new
 *
 * Allocates and returns a new 2D bitmap with the given width
 * and height. All bits are initialized to 0. The caller is
 * responsible for freeing the returned bitmap via Bit2_free.
 *
 * CRE: width <= 0 or height <= 0.
 * CRE: memory allocation failure.
 */
extern T Bit2_new(int width, int height);

/*
 * Bit2_free
 *
 * Deallocates all memory associated with *bit2 and sets *bit2
 * to NULL. The caller relinquishes ownership of the bitmap.
 *
 * CRE: bit2 is NULL or *bit2 is NULL.
 */
extern void Bit2_free(T *bit2);

/*
 * Bit2_width
 *
 * Returns the width (number of columns) of the bitmap.
 *
 * CRE: bit2 is NULL.
 */
extern int Bit2_width(T bit2);

/*
 * Bit2_height
 *
 * Returns the height (number of rows) of the bitmap.
 *
 * CRE: bit2 is NULL.
 */
extern int Bit2_height(T bit2);

/*
 * Bit2_get
 *
 * Returns the bit value (0 or 1) at position (col, row).
 *
 * CRE: bit2 is NULL.
 * CRE: col or row is out of bounds.
 */
extern int Bit2_get(T bit2, int col, int row);

/*
 * Bit2_put
 *
 * Sets the bit at position (col, row) to value and returns
 * the previous bit value that was stored at that position.
 *
 * CRE: bit2 is NULL.
 * CRE: col or row is out of bounds.
 * CRE: value is not 0 or 1.
 */
extern int Bit2_put(T bit2, int col, int row, int value);

/*
 * Bit2_applyfun
 *
 * Function pointer type for the apply function used by mapping
 * operations. Called once for each bit in the bitmap.
 */
typedef void Bit2_applyfun(int col, int row, T bit2, int elem,
                           void *cl);

/*
 * Bit2_map_col_major
 *
 * Calls the apply function for each bit in column-major order:
 * all rows of column 0, then column 1, etc.
 *
 * CRE: bit2 is NULL or apply is NULL.
 */
extern void Bit2_map_col_major(T bit2, Bit2_applyfun *apply,
                               void *cl);

/*
 * Bit2_map_row_major
 *
 * Calls the apply function for each bit in row-major order:
 * all columns of row 0, then row 1, etc.
 *
 * CRE: bit2 is NULL or apply is NULL.
 */
extern void Bit2_map_row_major(T bit2, Bit2_applyfun *apply,
                               void *cl);

#undef T
#endif
