/*
 *     uarray2.h
 *
 *     Interface for a 2D unboxed array (UArray2)
 *
 *     A UArray2 is a two-dimensional array of elements where:
 *     - Elements are stored UNBOXED (the array owns the memory for elements)
 *     - All elements have the same size
 *     - Elements are accessed by (column, row) coordinates
 *     - Valid indices: 0 <= col < width, 0 <= row < height
 *
 *     This is built on top of Hanson's UArray (1D unboxed array).
 */


#ifndef UARRAY2_INCLUDED
#define UARRAY2_INCLUDED

#define T UArray2_T
typedef struct T *T;

/*
 * UArray2_new
 *
 * Allocates and returns a new 2D array with the given dimensions.
 *
 * Parameters:
 *   width  - number of columns (must be >= 0)
 *   height - number of rows (must be >= 0)
 *   size   - size of each element in bytes (must be > 0)
 *
 * Returns: A new UArray2_T
 *
 * Notes:
 *   - All elements are initialized to zero bytes
 *   - It is a checked runtime error for width or height to be negative
 *   - It is a checked runtime error for size to be non-positive
 *   - Can raise Mem_Failed if allocation fails
 */
extern T
UArray2_new(int width, int height, int size);

/*
 * UArray2_free
 *
 * Deallocates the array and sets *uarray2 to NULL.
 *
 * Parameters:
 *   uarray2 - pointer to the UArray2_T to free
 *
 * Notes:
 *   - It is a checked runtime error for uarray2 or *uarray2 to be NULL
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
 * Returns: The width passed to UArray2_new
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
 * Returns: The height passed to UArray2_new
 */
extern int UArray2_height(T uarray2);

/*
 * UArray2_size
 *
 * Returns the size of each element in bytes.
 *
 * Parameters:
 *   uarray2 - the array to query
 *
 * Returns: The element size passed to UArray2_new
 */
extern int UArray2_size(T uarray2);

/*
 * UArray2_at
 *
 * Returns a pointer to the element at position (col, row).
 * The client casts this pointer and dereferences it to read/write.
 *
 * Parameters:
 *   uarray2 - the array
 *   col     - column index (0 <= col < width)
 *   row     - row index (0 <= row < height)
 *
 * Returns: void pointer to the element at (col, row)
 *
 * Notes:
 *   - It is a checked runtime error for col or row to be out of bounds
 *   - The returned pointer is valid until UArray2_free is called
 *
 * Example usage:
 *   int *elem = UArray2_at(arr, 3, 5);
 *   *elem = 42;  // write
 *   int val = *elem;  // read
 */
extern void *UArray2_at(T uarray2, int col, int row);

/*
 * Apply function type for mapping operations.
 *
 * Parameters:
 *   col    - current column index
 *   row    - current row index
 *   uarray2 - the array being traversed
 *   elem   - pointer to current element (same as UArray2_at(uarray2, col, row))
 *   cl     - closure: client data passed through from map call
 */
typedef void UArray2_applyfun(int col, int row, T uarray2, void *elem, void *cl);

/*
 * UArray2_map_row_major
 *
 * Calls the apply function for each element in row-major order.
 * Row-major means: visit all columns of row 0, then row 1, etc.
 *
 * Order: (0,0), (1,0), (2,0), ..., (width-1,0), (0,1), (1,1), ...
 *
 * Parameters:
 *   uarray2 - the array to traverse
 *   apply   - function to call for each element
 *   cl      - closure passed to each apply call
 */
extern void UArray2_map_row_major(T uarray2, UArray2_applyfun apply, void *cl);

/*
 * UArray2_map_col_major
 *
 * Calls the apply function for each element in column-major order.
 * Column-major means: visit all rows of column 0, then column 1, etc.
 *
 * Order: (0,0), (0,1), (0,2), ..., (0,height-1), (1,0), (1,1), ...
 *
 * Parameters:
 *   uarray2 - the array to traverse
 *   apply   - function to call for each element
 *   cl      - closure passed to each apply call
 */
extern void UArray2_map_col_major(T uarray2, UArray2_applyfun apply, void *cl);

#undef T
#endif
