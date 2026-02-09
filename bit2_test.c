/*
 * bit2_test.c
 *
 * Authors: Toye Adebayo & Teny Makuach
 * Date: 2/2/2026
 *
 * Purpose: Basic test program for the Bit2 interface. Tests
 *          creation, put, get, and freeing of a Bit2_T.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bit2.h"

int main(void)
{
        printf("Starting ... \n");

        Bit2_T newbitmap = Bit2_new(7, 5);
        assert(newbitmap != NULL);

        Bit2_put(newbitmap, 0, 1, 1);
        Bit2_put(newbitmap, 0, 0, 0);

        int val  = Bit2_get(newbitmap, 0, 1);
        int val1 = Bit2_get(newbitmap, 0, 0);

        printf("%d\n", val);
        printf("%d\n", val1);

        Bit2_free(&newbitmap);

        printf("End\n");
        return EXIT_SUCCESS;
}
