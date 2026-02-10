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
 * name: clean_close
 *
 * description: Cleans up and releases all allocated resources
 * before the program exits. Frees the board array, frees the
 * reader object, and closes the file (unless it is stdin).
 * This prevents memory leaks.
 *
 * Parameters:
 *   board  - UArray2 containing the sudoku puzzle
 *   reader - Pnmrdr object used to read the file
 *   fp     - File pointer to the input file
 *
 * Returns:
 *   void
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
 * name: validate_col
 *
 * description: Checks if a column in the sudoku board is valid.
 * A valid column must have all digits 1-9 appearing exactly once
 * with no repeats and no values outside the 1-9 range.
 *
 * Parameters:
 *   board - UArray2 containing the sudoku puzzle
 *   col   - column number to check (0-8)
 *
 * Returns:
 *   true if the column is valid, false if any digit repeats,
 *   is out of range, or missing from the column
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
 * name: validate_row
 *
 * description: Checks if a row in the sudoku board is valid.
 * A valid row must have all digits 1-9 appearing exactly once
 * with no repeats and no values outside the 1-9 range.
 *
 * Parameters:
 *   board - UArray2 containing the sudoku puzzle
 *   row   - row number to check (0-8)
 *
 * Returns:
 *   true if the row is valid, false if any digit repeats,
 *   is out of range, or missing from the row
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
 * name: validate_box
 *
 * description: Checks if a 3x3 box in the sudoku board is valid.
 * A valid box must have all digits 1-9 appearing exactly once with
 * no repeats and no values outside the 1-9 range. The box to check
 * is identified by its position in the 3x3 grid of boxes.
 *
 * Parameters:
 *   board    - UArray2 containing the sudoku puzzle
 *   box_row  - row position of the box (0-2)
 *   box_col  - column position of the box (0-2)
 *
 * Returns:
 *   true if the box is valid, false if any digit repeats,
 *   is out of range, or missing from the box
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
 * name: main
 *
 * description: Reads a sudoku puzzle from an image file and checks
 * if it is a valid solved sudoku. The image must be a 9x9 grayscale
 * image (PGM format) where pixel values are the sudoku digits. For
 * each row, column, and 3x3 box, the program checks that all digits
 * 1-9 appear exactly once with no repeats.
 *
 * Parameters:
 *   argc - number of command-line arguments (must be 1 or 2)
 *   argv - array of command-line arguments (optional filename)
 *
 * Returns:
 *   EXIT_SUCCESS (0) if sudoku is valid and fully solved,
 *   EXIT_FAILURE (1) if sudoku is invalid or has duplicates
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
