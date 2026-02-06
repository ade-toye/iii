/*
 * uarray2.c
 *
 * Authors: Toye Adebayo & Teny Makuach
 * Date: 2/2/2026
 *
 * Purpose: Implements UArray2, a 2D unboxed array interface. Provides 
 * functions to create/free a 2D array, get data (width/height/size), 
 * access an element by (col,row), and map over all elements in 
 * row-major or column-major order.
 * 
 * Key Insight: The 2D array is stored as one 1D UArray. Each (col,row) 
 * maps to a single index with index = row * width + col, and UArray_at 
 * returns the pointer to that element. The map functions only change 
 * traversal order, not how elements are stored.
*/

#include <stdlib.h>
#include "uarray2.h"
#include "uarray.h"
#include "assert.h"
#include "mem.h"

#define T UArray2_T
struct T{
    int width;
    int height;
    int size;
    UArray_T data;
};

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



 // diff uarray
 // indentation



/*
 * name: UArray2_new
 *
 * purpose:
 *     Allocate and return a new 2D unboxed array with the given width, height,
 *     and element size.
 *
 * parameters:
 *     width  - number of columns in the array; must be > 0
 *     height - number of rows in the array; must be > 0
 *     size   - size (in bytes) of each element; must be > 0
 *
 * returns:
 *     A new UArray2_T representing a width-by-height grid of elements.
 *
 * checked runtime errors (CRE):
 *     Raises a CRE if width <= 0, height <= 0, or size <= 0.
 *     Raises a CRE if any required memory allocation fails (for the UArray2
 *     struct or the underlying storage).
 */
 T UArray2_new(int width, int height, int size){
    assert(width > 0);
    assert(height > 0);
    assert(size > 0);
    // allocate memory for the struct
    T uarray2;
    NEW(uarray2);
    uarray2->width = width;
    uarray2->height = height;
    uarray2->size = size;
    uarray2->data = UArray_new(width * height, size);
    
    return uarray2;
}


/*
 * name: *UArray2_at
 * purpose: It allocates the index position (col, row) of an elemenent
 * parameters: T uarray2 - the uarray2_T to access the data, 
 *             int col - column index, and int row - row index
 * return:  A pointer element poin the uarray2   
 * effect: It returns a pointer to the elemment at position (col,row)
 */
void *UArray2_at(T uarray2, int col, int row){
    assert(uarray2 != NULL);
    assert(col >= 0 && col < uarray2->width);
    assert(row >= 0 && row < uarray2->height);
    // convert the 2D(col, row) into 1D
    int index = row * uarray2->width + col;

    return UArray_at(uarray2->data, index);
}


/*
 * name: UArray2_height
 * purpose: It tells the height of the 2D array
 * parameters: T uarray2 - the uarray2_T to access the data
 * return: It returns the height of the 2D array
 * effect: It accesses the height of the 2D array from the struct
 */
int UArray2_height(T uarray2){
    assert(uarray2 != NULL);
    return uarray2->height;
}


/*
 * name: UArray2_size
 * purpose: returns the stored size metadata of the struct
 * parameters: T uarray2 - 2D uarray
 * return: the byte size of the uarray2
 * effect: It accesses the size of each element in the 2D array 
 *         from the struct
 */
int UArray2_size(T uarray2){
    assert(uarray2 != NULL);
    return uarray2->size;
}



/*
 * name: UArray2_width
 * purpose: It tells us the width of the 2D array
 * parameters: T uarray2 - the uarray2D whose data being accessed
 * return: return the width metadata of the uarray2
 * effect: It accesses the width of the 2D array from the struct
 */
int UArray2_width(T uarray2){
    assert(uarray2 != NULL);
    return uarray2->width;
}


/*
 * name: UArray2_free
 * purpose: It deallocates the memory of the uarray2 
 * parameters: T *uarray2 - pointer to the uarray2_T to free 
 * return: void
 * effect: It frees the memory allocated the uarray2 
 */
void* UArray2_free(T *uarray2){
    assert(uarray2 != NULL);
    assert(*uarray2 != NULL);
    // free the 1D array 
    UArray_free(&(*uarray2)->data);
    // free the struct
    FREE(*uarray2);
    return NULL;
}


/*
 * name: UArray2_map_col_major
 * purpose: It applies the apply function in column-major order 
 * parameters: T uarray2 - the 2D uarray, UArray2-applyfun *apply - 
 *             function pointer, void *cl - closure
 * return: void 
 * effect: It traverses the 2D array in column-major order and applies 
 */
void UArray2_map_col_major(T uarray2, UArray2_applyfun *apply, 
                                  void *cl){
    // test checks
    assert(uarray2 != NULL);
    assert(apply != NULL); 
    // nested loops to traverse column-major
    for (int col = 0; col < uarray2->width; col++){
        for (int row = 0; row < uarray2->height; row++){
            void *elem = UArray2_at(uarray2, col, row);
            apply(col, row, uarray2, elem, cl);
        }
    }
}


/*
 * name: UArray2_map_row_major
 * purpose: It applies the apply function in row-major order 
 * parameters: T uarray2 - the 2D uarray, UArray2-applyfun apply 
 *             - function pointer, void *cl - closure 
 * return: void 
 * effect: It traverses the 2D array in row-major order and applies
 */
void UArray2_map_row_major(T uarray2, UArray2_applyfun *apply, void *cl){
    
    // test checks
    assert(uarray2 != NULL);
    assert(apply != NULL);
    // nested loops to traverse row-major
    for (int row = 0; row < uarray2->height; row++){
        for (int col = 0; col < uarray2->width; col++){
            void *elem = UArray2_at(uarray2, col, row);
            apply(col, row, uarray2, elem, cl);
        }
    }
}