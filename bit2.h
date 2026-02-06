/*
 * bit2.h
 *
 * Authors: Toye Adebayo & Teny Makuach
 * Date: 1/31/2026
 *
 * Purpose: Defines the public interface for Bit2, a two-dimensional 
 *      bitmap whose elements are bits (0/1). It supports creating 
 *      and freeing a width-by-height grid of bits, reading and 
 *      writing individual bits using (col, row) coordinates, and 
 *      traversing all bits in either row-major or column-major order.
 *
 * Key Insight: Bit2 is designed to save space by storing pixels as 
 *      packed bits, which is especially useful for PBM-style black/
 *      white images. Because a single bit does not have its own address 
 *      in memory, the interface uses put/get rather than an 'at' 
 *      function that returns a pointer. The mapping functions provide a 
 *      clean, consistent way to process every bit in the bitmap without 
 *      rewriting traversal loops.
 */
#ifndef BIT2_INCLUDED
#define BIT2_INCLUDED


#define T Bit2_T
typedef struct T *T;


/*
 * name: Bit2_new
 * purpose: Creates a new bit map struct that holds the metadata 
 * parameters: int width - number of columns in the bit map, 
 *             int height - number of rows in the bit map
 * return: Bit2 - a new Bit2_T struct
 * effect: it creates a Bit2 struct that holds the bits
 */
extern T Bit2_new(int width, int height);


/*
 * name: Bit2_get
 * purpose: It gets the value of a bit at a given position
 * parameters: T bit2 - the Bit2 struct to access, 
 *             int col - column index of the bit, 
 *             int row - row index of the bit 
 * return: Returns the value of a bit at a given position
 * effect: it accesses the bit at position (col,row)
 */
extern int Bit2_get(T bit2, int col, int row);


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
extern int Bit2_put(T bit2, int col, int row, int value);


/*
 * name: Bit2_width
 * purpose: It gets the width of a given bit2 struct 
 * parameters: T bit2 - the Bit2 struct whose width is being queried
 * return: Returns the width of a given bit2 struct
 * effect: It accesses the width data from the struct 
 */
extern int Bit2_width(T bit2);


/*
 * name: Bit2_height 
 * purpose: It gets the height of a given but struct
 * parameters: T bit2 - the Bit2 struct whose height is being queried
 * return: Returns the height of a goven bit2 struct
 * effect: It accesses the height data from the struct 
 */
extern int Bit2_height(T bit2);


/*
 * name: Bit2_applyfun
 * purpose: It is a function pointer type for the apply function 
 * parameters: int col - column index of the current bit, 
 *             int row - row index of the current bit, 
 *             T bit2 - the Bit2 struct being traversed, 
 *             int elem - the current bit value (0 or 1), 
 *             void *cl - closure data passed to each call
 * return: void 
 * effect: It is used to apply a function to each element in the bit2  
 */
typedef void Bit2_applyfun(int col, int row, T bit2, int elem, void *cl);


/*
 * name: Bit2_map_col_major
 * purpose: It applies the apply function in col-major order
 * parameters: T bit2 - the Bit2 struct to traverse, 
 *             Bit2_applyfun *apply - function to apply to each bit, 
 *             void *cl - closure data passed to apply
 * return: void
 * effect: It traverses the Bit2 in column-major order and calls apply
 */
extern void Bit2_map_col_major(T bit2, Bit2_applyfun *apply, void *cl);


/*
 * name: map_row_major
 * purpose: It is used to traverse the 2D bit array in row-major format
 * parameters: T bit2 - the Bit2 struct to traverse, 
 *             Bit2_applyfun *apply - function to apply to each bit, 
 *             void *cl - closure data passed to apply 
 * return: void
 * effect: It applies the apply function in row-major order 
 */
extern void Bit2_map_row_major(T bit2, Bit2_applyfun *apply, void *cl);


/*
 * name: Bit2_free
 * purpose: It deallocates the memory of the bit2 struct
 * parameters: T *bit2 - pointer to the Bit2_T to free
 * return: void 
 * effect: It frees the memory allocated to the bit2 struct
 */
extern void Bit2_free(T *bit2);


#undef T
#endif
