/*
 * sudoku.c
 *
 * Authors: Toye Adebayo & Teny Makuach
 * Date: 2/4/2026
 *
 * Purpose: Reads a 9x9 graymap file and determines whether it
 *          represents a valid solved Sudoku puzzle. Exits with
 *          EXIT_SUCCESS (0) if valid, EXIT_FAILURE (1) if not.
 *          Produces no output on stdout.
 *
 * Key Insight: A solved Sudoku has digits 1-9 appearing exactly
 *          once in every row, every column, and every 3x3 box.
 *          We validate each constraint with a boolean seen-array
 *          to detect duplicates.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "pnmrdr.h"
#include "assert.h"
#include "uarray2.h"

#define DIM 9
#define BOX 3

/*
 * clean_close
 *
 * Frees the UArray2 board by dereferencing the pointer.
 * Frees the Pnmrdr reader. If the file pointer is not stdin,
 * closes the file. Called before every program exit to ensure
 * all resources are released and prevent memory leaks.
 *
 * CRE: board, reader, or fp are NULL.
 */
static void clean_close(UArray2_T board, Pnmrdr_T reader,
                         FILE *fp)
{
        UArray2_free(&board);
        Pnmrdr_free(&reader);
        if (fp != stdin) {
                fclose(fp);
        }
}

/*
 * validate_col
 *
 * Checks whether column col contains all digits 1 through 9
 * with no duplicates and no out-of-range values. Uses a seen
 * array to track which digits have been encountered. Returns
 * true if the column is valid, false if any digit is invalid
 * or repeated.
 *
 * CRE: col < 0 or col >= DIM.
 * CRE: board is NULL or not fully initialized.
 */
static bool validate_col(UArray2_T board, int col)
{
        bool seen[DIM + 1] = {false};
        for (int row = 0; row < DIM; row++) {
                int val = *(int *)UArray2_at(board, col, row);
                if (val < 1 || val > DIM || seen[val]) {
                        return false;
                }
                seen[val] = true;
        }
        return true;
}

/*
 * validate_row
 *
 * Checks whether row row contains all digits 1 through 9
 * with no duplicates and no out-of-range values. Uses a seen
 * array to track which digits have been encountered. Returns
 * true if the row is valid, false if any digit is invalid
 * or repeated.
 *
 * CRE: row < 0 or row >= DIM.
 * CRE: board is NULL or not fully initialized.
 */
static bool validate_row(UArray2_T board, int row)
{
        bool seen[DIM + 1] = {false};
        for (int col = 0; col < DIM; col++) {
                int val = *(int *)UArray2_at(board, col, row);
                if (val < 1 || val > DIM || seen[val]) {
                        return false;
                }
                seen[val] = true;
        }
        return true;
}

/*
 * validate_box
 *
 * Checks whether the 3x3 sub-grid at position (box_row,
 * box_col) contains all digits 1 through 9 with no duplicates
 * and no out-of-range values. The box starts at pixel
 * (box_col * 3, box_row * 3). Uses a seen array to track
 * which digits have been encountered. Returns true if valid,
 * false otherwise.
 *
 * CRE: box_row < 0 or box_row >= BOX (3).
 * CRE: box_col < 0 or box_col >= BOX (3).
 * CRE: board is NULL or not fully initialized.
 */
static bool validate_box(UArray2_T board, int box_row,
                          int box_col)
{
        bool seen[DIM + 1] = {false};
        int start_row = box_row * BOX;
        int start_col = box_col * BOX;

        for (int r = 0; r < BOX; r++) {
                for (int c = 0; c < BOX; c++) {
                        int row = start_row + r;
                        int col = start_col + c;
                        int val = *(int *)UArray2_at(board,
                                                     col, row);
                        if (val < 1 || val > DIM || seen[val]) {
                                return false;
                        }
                        seen[val] = true;
                }
        }
        return true;
}

/*
 * main
 *
 * Reads a 9x9 graymap (PGM) file from a file (if provided as
 * argument) or from stdin. Loads all pixel values as digits.
 * Validates that all 9 rows contain 1-9 with no duplicates.
 * Validates that all 9 columns contain 1-9 with no duplicates.
 * Validates that all nine 3x3 boxes contain 1-9 with no
 * duplicates. Returns EXIT_SUCCESS (0) if all validations pass,
 * EXIT_FAILURE (1) if any validation fails. Accepts 0 or 1
 * command-line argument.
 *
 * CRE: more than 1 command-line argument provided.
 * CRE: input file cannot be opened.
 * CRE: input is not a 9x9 graymap with denominator 9.
 */
int main(int argc, char *argv[])
{
        FILE *fp = NULL;

        assert(argc <= 2);

        if (argc == 2) {
                fp = fopen(argv[1], "r");
                assert(fp != NULL);
        } else {
                fp = stdin;
        }

        Pnmrdr_T reader = Pnmrdr_new(fp);
        Pnmrdr_mapdata data = Pnmrdr_data(reader);

        assert(data.type == Pnmrdr_gray);
        assert(data.width == (unsigned)DIM);
        assert(data.height == (unsigned)DIM);
        assert(data.denominator == (unsigned)DIM);

        UArray2_T board = UArray2_new(DIM, DIM, sizeof(int));

        /* Read all pixels into the board */
        for (int row = 0; row < DIM; row++) {
                for (int col = 0; col < DIM; col++) {
                        int pixel = Pnmrdr_get(reader);
                        *(int *)UArray2_at(board, col, row) =
                                pixel;
                }
        }

        /* Validate all rows and columns */
        for (int i = 0; i < DIM; i++) {
                if (!validate_row(board, i) ||
                    !validate_col(board, i)) {
                        clean_close(board, reader, fp);
                        return EXIT_FAILURE;
                }
        }

        /* Validate all 3x3 boxes */
        for (int br = 0; br < BOX; br++) {
                for (int bc = 0; bc < BOX; bc++) {
                        if (!validate_box(board, br, bc)) {
                                clean_close(board, reader, fp);
                                return EXIT_FAILURE;
                        }
                }
        }
        clean_close(board, reader, fp);
        return EXIT_SUCCESS;
}
