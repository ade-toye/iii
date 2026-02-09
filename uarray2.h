/*
 * uarray2.h
 *
 * Authors: Toye Adebayo & Teny Makuach
 * Date: 1/31/2026
 *
 * Purpose: Defines the public interface for UArray2, a 2D
 *          "unboxed" array abstraction. This provides functions to
 *          create and free a 2D array, query its dimensions and
 *          element size, access an element by (col, row), and
 *          traverse all elements in either row-major or
 *          column-major order using an apply function.
 *
 * Key Insight: Although UArray2 behaves like a true 2D grid, it
 *          is implemented on top of a 1D unboxed UArray
 *          (Hanson's UArray_T). Each (col, row) location is
 *          mapped to the correct underlying storage address,
 *          allowing efficient memory use while still presenting
 *          a clean 2D interface.
 */

#ifndef UARRAY2_INCLUDED
#define UARRAY2_INCLUDED

#define T UArray2_T
typedef struct T *T;

/*
 * UArray2_new
 *
 * Allocates and returns a new 2D unboxed array with the given
 * width, height, and element size. All elements are initialized
 * to zero bytes. The caller is responsible for freeing the
 * returned array by calling UArray2_free.
 *
 * Parameters:
 *   width  - number of columns in the array; must be > 0
 *   height - number of rows in the array; must be > 0
 *   size   - size (in bytes) of each element; must be > 0
 *
 * Returns: A new UArray2_T representing a width-by-height grid.
 *
 * CRE: width <= 0, height <= 0, or size <= 0.
 * CRE: memory allocation failure.
 */
extern T UArray2_new(int width, int height, int size);

/*
 * UArray2_free
 *
 * Deallocates all memory associated with *uarray2 and sets
 * *uarray2 to NULL. The caller relinquishes ownership of the
 * array.
 *
 * Parameters:
 *   uarray2 - pointer to the UArray2_T to free
 *
 * CRE: uarray2 is NULL or *uarray2 is NULL.
 */
extern void UArray2_free(T *uarray2);

/*
 * UArray2_width
 *
 * Returns the width (number of columns) of the array.
 *
 * Parameters:
 *   uarray2 - the array to query
 *
 * Returns: The width passed to UArray2_new.
 *
 * CRE: uarray2 is NULL.
 */
extern int UArray2_width(T uarray2);

/*
 * UArray2_height
 *
 * Returns the height (number of rows) of the array.
 *
 * Parameters:
 *   uarray2 - the array to query
 *
 * Returns: The height passed to UArray2_new.
 *
 * CRE: uarray2 is NULL.
 */
extern int UArray2_height(T uarray2);

/*
 * UArray2_size
 *
 * Returns the size (in bytes) of each element in the array.
 *
 * Parameters:
 *   uarray2 - the array to query
 *
 * Returns: The element size passed to UArray2_new.
 *
 * CRE: uarray2 is NULL.
 */
extern int UArray2_size(T uarray2);

/*
 * UArray2_at
 *
 * Returns a pointer to the element at position (col, row).
 * The caller casts and dereferences this pointer to read or
 * write. The returned pointer is valid until UArray2_free is
 * called.
 *
 * Parameters:
 *   uarray2 - the array
 *   col     - column index (0 <= col < width)
 *   row     - row index (0 <= row < height)
 *
 * Returns: void pointer to the element at (col, row).
 *
 * CRE: uarray2 is NULL.
 * CRE: col or row is out of bounds.
 */
extern void *UArray2_at(T uarray2, int col, int row);

/*
 * UArray2_applyfun
 *
 * Function pointer type for the apply function used by mapping
 * operations. Called once for each element in the array.
 *
 * Parameters:
 *   col     - current column index
 *   row     - current row index
 *   uarray2 - the array being traversed
 *   elem    - pointer to current element
 *   cl      - closure data passed through from the map call
 */
typedef void UArray2_applyfun(int col, int row, T uarray2,
                              void *elem, void *cl);

/*
 * UArray2_map_col_major
 *
 * Calls the apply function for each element in column-major
 * order: all rows of column 0, then column 1, etc.
 *
 * Parameters:
 *   uarray2 - the array to traverse
 *   apply   - function to call for each element
 *   cl      - closure passed to each apply call
 *
 * CRE: uarray2 is NULL or apply is NULL.
 */
extern void UArray2_map_col_major(T uarray2,
                                  UArray2_applyfun *apply,
                                  void *cl);

/*
 * UArray2_map_row_major
 *
 * Calls the apply function for each element in row-major
 * order: all columns of row 0, then row 1, etc.
 *
 * Parameters:
 *   uarray2 - the array to traverse
 *   apply   - function to call for each element
 *   cl      - closure passed to each apply call
 *
 * CRE: uarray2 is NULL or apply is NULL.
 */
extern void UArray2_map_row_major(T uarray2,
                                  UArray2_applyfun *apply,
                                  void *cl);

#undef T
#endif
