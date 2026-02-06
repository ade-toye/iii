/*
 * bit2.c
 *
 * Authors: Toye Adebayo & Teny Makuach
 * Date: 2/2/2026
 *
 * Purpose: 
 * 
 * Key Insight: 
*/

#include <stdio.h>
#include <stdlib.h>
#include "bit2.h"
#include "bit.h"
#include "mem.h"
#include "assert.h"

#define T Bit2_T 
struct T{
    int width;
    int height;
    Bit_T data;
};

/*
 * name: Bit2_new
 * purpose: Creates a new bit map struct that holds the metadata 
 * parameters: int width - number of columns in the bit map, 
 *             int height - number of rows in the bit map
 * return: Bit2 - a new Bit2_T struct
 * effect: it creates a Bit2 struct that holds the bits
 */
T Bit2_new(int width, int height){
    // Test checks 
    assert(width > 0);
    assert(height > 0);

    T bit_2;
    NEW(bit_2);

    bit_2->width = width;
    bit_2->height = height;

    bit_2->data = Bit_new(width * height);
    return bit_2;
}


/*
 * name: Bit2_get
 * purpose: It gets the value of a bit at a given position
 * parameters: T bit2 - the Bit2 struct to access, 
 *             int col - column index of the bit, 
 *             int row - row index of the bit 
 * return: Returns the value of a bit at a given position
 * effect: it accesses the bit at position (col,row)
 */
int Bit2_get(T bit2, int col, int row){
    // Test checks 
    assert(bit2 != NULL);
    assert(col >= 0 && col < bit2->width);
    assert(row >= 0 && row < bit2->height);
    
    int index = row * bit2->width + col;

    return Bit_get(bit2->data, index);
}


/*
 * name: Bit2_put
 * purpose: It inserts the bit value in the Bit2 struct data
 * parameters: T bit2 - the Bit2 struct to update, 
 *             int col - column index of the bit, 
 *             int row - row index of the bit, 
 *             int value - bit value (0 or 1) to store
 * return: bit2 - the previous bit value stored at that position
 * effect: it accesses the index of the bit and inserts the value
 */
int Bit2_put(T bit2, int col, int row, int value){
    // Test checks 
    assert(bit2 != NULL);
    assert(col >= 0 && col < bit2->width);
    assert(row >= 0 && row < bit2->height);
    assert((value == 0) || (value == 1));

    int index = row * bit2->width + col;

    return Bit_put(bit2->data, index, value);
}


/*
 * name: Bit2_width
 * purpose: It gets the width of a given bit2 struct
 * parameters: T bit2 - the Bit2 struct whose width is being queried
 * return: Returns the width of a given bit2 struct
 * effect: It accesses the width data from the struct 
 */
int Bit2_width(T bit2){
    // Test checks 
    assert(bit2 != NULL);
    return bit2->width;
}


/*
 * name: Bit2_height 
 * purpose: It gets the height of a given but struct
 * parameters: T bit2 - the Bit2 struct whose height is being queried
 * return: Returns the height of a goven bit2 struct
 * effect: It accesses the height data from the struct 
 */
int Bit2_height(T bit2){
    // Test checks 
    assert(bit2 != NULL);
    return bit2->height;
}




/*
 * name: Bit2_map_col_major
 * purpose: It applies the apply function in col-major order
 * parameters: T bit2 - the Bit2 struct to traverse, 
 *             Bit2_applyfun *apply - function to apply to each bit, 
 *             void *cl - closure data passed to apply
 * return: void
 * effect: It traverses the Bit2 in column-major order and calls apply
 */

void Bit2_map_col_major(T bit2, Bit2_applyfun *apply, void *cl){
    // test checks
    assert(bit2 != NULL);
    assert(apply != NULL);

    for (int col = 0; col < bit2->width; col++){
        for(int row = 0; row < bit2->height; row++){
            int elem = Bit2_get(bit2, col, row);
            apply(col, row, bit2, elem, cl);
        }
    }
}


/*
 * name: map_row_major
 * purpose: It is used to traverse the 2D bit array in row-major format
 * parameters: T bit2 - the Bit2 struct to traverse, 
 *             Bit2_applyfun *apply - function to apply to each bit, 
 *             void *cl - closure data passed to apply 
 * return: void
 * effect: It applies the apply function in row-major order 
 */
void Bit2_map_row_major(T bit2, Bit2_applyfun *apply, void *cl){
    // test checks
    assert(bit2 != NULL);
    assert(apply != NULL);

    for (int row = 0; row < bit2->height; row++){
        for(int col = 0; col < bit2->width; col++){
            int elem = Bit2_get(bit2, col, row);
            apply(col, row, bit2, elem, cl);
        }
    }
}


/*
 * name: Bit2_free
 * purpose: It deallocates the memory of the bit2 struct
 * parameters: T *bit2 - pointer to the Bit2_T to free
 * return: void 
 * effect: It frees the memory allocated to the bit2 struct
 */
void Bit2_free(T *bit2){
    // Test checks 
    assert(bit2 != NULL);
    assert(*bit2 != NULL);

    Bit_free(&((*bit2)->data));

    FREE(*bit2);
    *bit2 = NULL;

}
