/*
 * uarray2.h
 *
 * Authors: Toye Adebayo & Teny Makuach
 * Date: 1/31/2026
 *
 * Purpose: Defines the public interface for UArray2, a 2D 
 *  “unboxed” array abstraction. This provides functions to create 
 *  and free a 2D array, query its dimensions and element size, 
 *  access an element by (col, row), and traverse all elements in 
 *  either row-major or column-major order using a apply function.
 * 
 * Key Insight: Although UArray2 behaves like a true 2D grid, it 
 *   is implemented on top of a 1D unboxed UArray (Hanson’s UArray_T). 
 *   Each (col, row) location is mapped to the correct underlying 
 *   storage address, allowing efficient memory use while still 
 *   presenting a clean 2D interface.
 */
 
#ifndef UARRAY2_INCLUDED
#define UARRAY2_INCLUDED


/* Declare the struct*/
#define T UArray2_T
typedef struct T *T;


/*
 * name: UArray2_new
 * purpose: Allocate and return a new 2D unboxed array with the 
 * given width, height, and element size.
 *
 * parameters:
 * width  - number of columns in the array; must be > 0
 * height - number of rows in the array; must be > 0
 * size   - size (in bytes) of each element; must be > 0
 *
 * returns:
 * A new UArray2_T representing a width-by-height grid of elements.
 *
 * checked runtime errors (CRE):
 * Raises a CRE if width <= 0, height <= 0, or size <= 0.
 * Raises a CRE if any required memory allocation fails (for the UArray2
 * struct or the underlying storage).
 */
extern T UArray2_new(int width, int height, int size);


/*
 * name: *UArray2_at
 * purpose: It allocates the index position (col, row) of an elemenent
 * parameters: T uarray2 - the uarray2_T to access the data, 
 *             int col - column index, and int row - row index
 * return:  A pointer element poin the uarray2   
 * effect: It returns a pointer to the elemment at position (col,row)
 */
extern void *UArray2_at(T uarray2, int col, int row);


/*
 * name: UArray2_height
 * purpose: It tells the height of the 2D array
 * parameters: T uarray2 - the uarray2_T to access the data
 * return: It returns the height of the 2D array
 * effect: It accesses the height of the 2D array from the struct
 */
extern int UArray2_height(T uarray2);


/*
 * name: UArray2_size
 * purpose: returns the stored size metadata of the struct
 * parameters: T uarray2 - 2D uarray
 * return: the byte size of the uarray2
 * effect: It accesses the size of each element in the 2D array 
 *         from the struct
 */
extern int UArray2_size(T uarray2);


/*
 * name: UArray2_width
 * purpose: It tells us the width of the 2D array
 * parameters: T uarray2 - the uarray2D whose data being accessed
 * return: return the width metadata of the uarray2
 * effect: It accesses the width of the 2D array from the struct
 */
extern int UArray2_width(T uarray2);


/*
 * name: UArray2_free
 * purpose: It deallocates the memory of the uarray2 
 * parameters: T *uarray2 - pointer to the uarray2_T to free 
 * return: void
 * effect: It frees the memory allocated the uarray2 
 */
extern void* UArray2_free(T *uarray2);


/*
 * name: UArray2_applyfun
 * purpose: It is a function that that applies the function type for the 
 *          mapping operation
 * parameters: int col, int row, T uarray2, void *elem, void *cl
 * return: void 
 * effect: It applies the the function mapping to both map_col_major and
 *         map_row_major
 */
typedef void UArray2_applyfun(int col, int row, T uarray2, void *elem, 
                              void *cl);


/*
 * name: UArray2_map_col_major
 * purpose: It applies the apply function in column-major order 
 * parameters: T uarray2 - the 2D uarray, UArray2-applyfun *apply - 
 *             function pointer, void *cl - closure
 * return: void 
 * effect: It traverses the 2D array in column-major order and applies 
 */
extern void UArray2_map_col_major(T uarray2, UArray2_applyfun *apply, 
                                  void *cl);


/*
 * name: UArray2_map_row_major
 * purpose: It applies the apply function in row-major order 
 * parameters: T uarray2 - the 2D uarray, UArray2-applyfun apply 
 *             - function pointer, void *cl - closure 
 * return: void 
 * effect: It traverses the 2D array in row-major order and applies
 */
extern void UArray2_map_row_major(T uarray2, UArray2_applyfun, void *cl);


#undef T
#endif // UARRAY2_INCLUDED
