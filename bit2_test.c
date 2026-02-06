#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bit2.h"


int main(){

    printf("Starting ... \n");
    Bit2_T newbitmap = Bit2_new(7, 5);
    assert(newbitmap != NULL);
    // int h = Bit2_height(newbitmap);
    // int w = Bit2_width(newbitmap);
    // for(int i = 0; i < ( * newbitmap->height); i++){
    //     printf("%d\n", i);
    // }
    // int bit_val = Bit2_get(newbitmap, 0,0);
    // int bit_val1 = Bit2_get(newbitmap, 1,1);
    // printf("%d\n", bit_val);
    // printf("%d\n", bit_val1);

    Bit2_put(newbitmap, 0, 1, 1);
    Bit2_put(newbitmap, 0, 0, 0);
    int val = Bit2_get(newbitmap, 0, 1);
    int val1 = Bit2_get(newbitmap, 0, 0);
    printf("%d\n", val);
    printf("%d\n", val1);
    printf("End\n");
    return 0;
}