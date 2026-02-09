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

 
 bool is_black(Bit2_T board, int col, int row){
    return Bit2_get(board, col, row) == 1;
 }

 void add_if_black(Bit2_T board, Bit2_T visited, int col, int row){
    if(col < 0 || row < 0 || col >= Bit2_width(board) || row >= Bit2_height(board)) return;
    if(!(is_black(board, col, row))) return;
    if(Bit2_get(visited, col, row)) return;

    Bit2_put(visited, col, row, 1);

    add_if_black(board, visited, col-1, row);
    add_if_black(board, visited, col+1, row);
    add_if_black(board, visited, col, row - 1);
    add_if_black(board, visited, col, row + 1);
    
 }

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
    Bit2_T visited = Bit2_new((int)data.width, (int)data.height);

    // Scan top and bottom borders (only border pixels)
    for (int col = 0; col < Bit2_width(board); col++) {
        add_if_black(board, visited, col, 0);                      // top row
        add_if_black(board, visited, col, Bit2_height(board) - 1); // bottom row
    }

    // Scan left and right borders (only border pixels)
    for (int row = 0; row < Bit2_height(board); row++) {
        add_if_black(board, visited, 0, row);                      // left column
        add_if_black(board, visited, Bit2_width(board) - 1, row);  // right column
    }

    
    // remove boarder connected pixels
     for(int col = 0; col < Bit2_width(board); col++){
        for(int row = 0; row < Bit2_height(board); row++){
            if(Bit2_get(visited, col, row)){
                Bit2_put(board, col, row, 0);
            }
        }
    }
    
     for(int col = 0; col < Bit2_width(board); col++){
        for(int row = 0; row < Bit2_height(board); row++){
            int value = Bit2_get(board, col, row);
        }

    }
    
    // Output the unblackedged image
    printf("P1\n%d %d\n", Bit2_width(board), Bit2_height(board));
    for(int col = 0; col < Bit2_width(board); col++){
        for(int row = 0; row < Bit2_height(board); row++){
            int value = Bit2_get(board, col, row);
        }
    }


    // logic
    // Iterate through the board
        // loop through the rows(height)
            // if is_black 
            // add_if_black
        // loop throught the columns
    

    // function to make it white if it is a black edge 

    // clean up memory
    clean_close(board, reader, fp);
    Bit2_free(&visited);
    
    return EXIT_SUCCESS;
}