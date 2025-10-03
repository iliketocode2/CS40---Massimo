#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "uarray2b.h"
#include "uarray2.h"
#include "uarray.h"
#include "except.h"

int main(int argc, char *argv[])
{
        (void) argc;
        (void) argv;

        printf("We are testing!\n");

        int width = 4;
        int height = 4;
        int size = 8;
        int blocksize = 2;

        printf("Putting in a width of %d, a height of %d, data size of %d, and a blocksize of %d\n", width, height, size, blocksize);
        UArray2b_T our_array = UArray2b_new(width, height, size, blocksize);

        printf("ABOUT TO FREE\n");
        free(our_array);

        return 0;
}