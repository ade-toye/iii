/*
 * sudoku.c
 *
 * Authors: Toye Adebayo & Teny Makuach
 * Date: 2/4/2026
 *
 * Purpose: 
 * 
 * Key Insight: 
*/
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "mem.h"
#include "pnmrdr.h"
#include "except.h"  // for error testing
#include "assert.h"
#include "uarray2.h"



/*
 * name: 
 * purpose:
 *
 * parameters:
 *
 * returns:
 *
 * checked runtime errors (CRE):
 */







/*
 * name: validate_col
 *
 * purpose:
 *
 * parameters:
 *
 * returns:
 *
 * checked runtime errors (CRE):
 */
int validate_col(UArray2_T board) {
    for (int col = 0; col < 9; col++) {
        bool seen[10] = {false};  // reset for each column
        for (int row = 0; row < 9; row++) {
            int value = *(int *)UArray2_at(board, col, row);
            if (value < 1 || value > 9) {
                return 0;  // invalid value
            }
            if (seen[value]) {
                return 0;  // duplicate in column
            }
            seen[value] = true;
        }
    }
    return 1;  // all columns valid
}




// int validate_row(UArray2_T board) {
//     for(int row = 0; row < data->height; row++){
//         for (int col = 0; col < data->width; col++){
            

//         }
        
        
//     }
// }


 int main(int argc, char *argv[]){

    // declare variables
    FILE *fp = NULL;
    // Handling arguments
    if (argc > 2){
        fprintf(stderr, "Usage: %s [input_file]\n", argv[0]);
        return EXIT_FAILURE;
    } else if (argc == 2){
        fp = fopen(argv[1], "r");
        if (fp == NULL) {
            fprintf(stderr, "Error: could not open %s\n", argv[0]);
            return EXIT_FAILURE;
        }
    } else {
        fp = stdin;          
    }

    // SetUp Pnmrdr
    Pnmrdr_T reader = Pnmrdr_new(fp);
    Pnmrdr_mapdata data = Pnmrdr_data(reader);

    // validate the graymap
    assert(data.type == Pnmrdr_gray);
    assert(data.width == 9);
    assert(data.height == 9);
    assert(data.denominator == 9);

    // Create a board
    UArray2_T board = UArray2_new(9, 9, sizeof(int));

    // Read pixels
    for (int row = 0; row < 9; row ++ ){
        for (int col = 0; col < 9; col++){
            int pixel = Pnmrdr_get(reader);
            int *cell = UArray2_at(board, col, row);

            // Put the pixels into the 9x9 table
            printf("%d\n", pixel);
            *cell = pixel; 
            assert(cell != NULL);
        }
    }

    
    // validate sudoku ()

    // check the number of rows
    for (int col = 0; col < 9; col++){
        for (int row = 0; row< 9; row++)
        if(!(validate_col(board))){
            return EXIT_FAILURE;
        }
    }

    // Check the number of columns

    // check that each subgrid is 3x3


    // clean the memory


    // exit with success

    return 0;
}