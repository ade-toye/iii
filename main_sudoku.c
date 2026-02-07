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
#include "pnmrdr.h"
#include "except.h"  // for error testing
#include "assert.h"
#include "uarray2.h"



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
static bool validate_col(UArray2_T board, int col) {
    bool seen[10] = {false};
    for (int row = 0; row < 9; row++) {
        int value = *(int *)UArray2_at(board, col, row);
        if (value < 1 || value > 9 || seen[value]) 
        {
            return false;  // invalid value
        }
        seen[value] = true;
    }
    return true;
}

static bool validate_row(UArray2_T board, int row) {
    bool seen[10] = {false};
    for (int col = 0; col < 9; col++) {
        int value = *(int *)UArray2_at(board, col, row);
        if (value < 1 || value > 9 || seen[value]) 
        {
            return false;  // invalid value
        }
        seen[value] = true;
    }
    return true;
}

static bool validate_box(UArray2_T board, int box_row, int box_col){
    bool seen[10] = {false};
    int start_row = box_row * 3;
    int start_col = box_col * 3;
    
    for (int r = 0; r < 3; r++){
        for (int c = 0; c < 3; c++){
            int row = start_row + r;
            int col = start_col + c;
            int value = *(int *)(UArray2_at(board, col, row));
            if (value < 1 || value > 9 || seen[value]){
                return false;
            }
            seen[value] = true;
        }
    }
    return true;
}

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
            assert(cell != NULL);
            *cell = pixel; 
        }
    }

    for (int box_row = 0; box_row < 3; box_row++) {
        for (int inner_row = 0; inner_row < 3; inner_row++) {

            for (int box_col = 0; box_col < 3; box_col++) {

                int start_row = box_row * 3;
                int start_col = box_col * 3;

                for (int inner_col = 0; inner_col < 3; inner_col++) {
                    int value = *(int *)UArray2_at(board,
                                                start_col + inner_col,
                                                start_row + inner_row);
                    printf("%d ", value);
                }
                printf("  ");      // space between 3x3 boxes
            }
            printf("\n");
        }
        printf("\n");             // blank line between box rows
    }

    // check the number of rows
    bool is_valid = true;

    for (int i = 0; i < 9; i++){
        if(!validate_col(board, i) || (!validate_row(board, i))){
            printf("invalid\n");
            
            return EXIT_FAILURE;
        } else {
            printf("valid\n");
        }
    }


    for (int board_col = 0; board_col < 3; board_col++) {
        for (int board_row = 0; board_row < 3; board_row++) {
            if(!validate_box(board, board_col, board_row)){
                is_valid = false;
            } 
        }
    }
    // close the file
    fclose(fp);
    // clean the memory
    Pnmrdr_free(&reader);
    UArray2_free(&board);

    if (!is_valid){
    return EXIT_FAILURE;
   }
   return EXIT_SUCCESS;
}