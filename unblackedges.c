/*
 * unblackedges.c
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
#include "pnmrdr.h"
#include "assert.h"
#include "bit2.h"
// #include "queue.h"
#include "mem.h"




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


 void clean_close(Bit2_T board, Pnmrdr_T reader, FILE *fp){
        Bit2_free(&board);
        Pnmrdr_free(&reader);
        if (fp != stdin) fclose(fp);
 }

 // is_black function() 

 // add_if_black()

 int main(int argc, char *argv[]){

    // declare variables
    FILE *fp = NULL;
    // Handling arguments
    if (argc > 2){
        fprintf(stderr, "Usage: %s [input_file]\n", argv[0]);
        return EXIT_FAILURE;
    } else if (argc == 2){
        fp = fopen(argv[1], "rb");
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
    if (data.type != Pnmrdr_bit) {
    fprintf(stderr, "Error: input is not a PBM bitmap\n");
    }
    assert(data.width > 0);
    assert(data.height > 0);

    // Create board
    Bit2_T board = Bit2_new((int)data.width, (int)data.height); 
    // Read pixels
    for(int row = 0; row < (int)data.height; row++){
        for(int col = 0; col < (int)data.width; col++){
            int pixel = Pnmrdr_get(reader);
            printf("pixel is: %d\n", pixel);
            Bit2_put(board, col, row, pixel); 
             
            // validate the pixel 
            if (!(pixel == 0 || pixel == 1)){
                fprintf(stderr, "Error: pixel value is not 0 or 1\n");
                clean_close(board, reader, fp);
                return EXIT_FAILURE;
            }
        }
    }

    // Scan top and bottom boarders

    // Scan left and right boarders


    // remove boarder connected pixels


    // Output the unblackedged image

    
    // logic
    // Iterate through the board
        // loop through the rows(height)
            // if is_black 
            // add_if_black
        // loop throught the columns
    

    // function to make it white if it is a black edge 

    // clean up memory
    Pnmrdr_free(&reader);
    Bit2_free(&board);
    if (fp != stdin) fclose(fp);
    
    return EXIT_SUCCESS;
}