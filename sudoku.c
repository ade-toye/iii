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
 * Frees the board, Pnmrdr reader, and closes the file if it is
 * not stdin. Called before every program exit to prevent leaks.
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
 * Returns true if column col of board contains digits 1-9 with
 * no duplicates or out-of-range values. Returns false otherwise.
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
 * Returns true if row row of board contains digits 1-9 with
 * no duplicates or out-of-range values. Returns false otherwise.
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
 * Returns true if the 3x3 sub-grid starting at
 * (box_col * 3, box_row * 3) contains digits 1-9 with no
 * duplicates or out-of-range values. Returns false otherwise.
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
