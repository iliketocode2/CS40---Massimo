/**************************************************************
 *
 *                     urray2b.c
 *
 *     Assignment: locality
 *     Authors:  William Goldman (wgoldm03) and Massimo Bottari (mbotta01) 
 *     Date:     October 2, 2025
 *
 *     This is the implementation for a 2D boxed array (UArray2b) module.
 *
 *
 **************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "uarray2b.h"
#include "uarray2.h"
#include "uarray.h"
#include "except.h"
#include "assert.h"

struct UArray2b_T {
        int width;
        int height;
        int blocksize;
        int size;
        int blocks_wide;
        int blocks_high;
        // UArray_T pixels;
        UArray2_T blocks;
};

void initialize_block(int col, int row, UArray2b_T array2b, void *elem,
                      void *size);

void Uarray2b_map(UArray2b_T array2b,
                          void apply(int col, int row, UArray2b_T array2b,
                                     void *elem, void *cl),
                          void *cl);

UArray2b_T UArray2b_new (int width, int height, int size, int blocksize)
{

        assert(blocksize >= 1 && size >= 1);
        assert(width >= 0 && height >= 0);

        UArray2b_T array = malloc(sizeof(*array));

        assert(array != NULL);

        array->width = width;
        array->height = height;
        array->blocksize = blocksize;
        array->size = size;
        
        int blocks_wide = width / blocksize;
        int blocks_high = height / blocksize;

        if ((float)width / (float)blocksize > width / blocksize) {
                blocks_wide++;
        }
        
        if ((float)height / (float)blocksize > height / blocksize) {
                blocks_high++;
        }

        array->blocks_wide = blocks_wide;
        array->blocks_high = blocks_high;
        
        array->blocks = UArray2_new(blocks_wide, blocks_high, sizeof(UArray_T));

        UArray2b_map(array, initialize_block, &size);

        return array;
}

void initialize_block(int col, int row, UArray2b_T array2b, void *elem,
                      void *size) 
{
        printf("accessed %d, %d\n", col, row);

        int array_Length = (UArray2b_blocksize(array2b)) * 
                           (UArray2b_blocksize(array2b));

        
        *(UArray_T *)elem = UArray_new(array_Length, *(int *)size);

        // UArray2_map_row_major(UArray2_T arr, void apply(int col, int row, 
        //                                 UArray2_T arr, void *elem, void *cl), 
        //                                 void *cl)

        // for (int i = 0; i < array_Length; i++) {
        //         UArray_at(*elem, i) = 
        // }
}


UArray2b_T UArray2b_new_64K_block(int width, int height, int size) 
{

        assert(size >= 1 && size <= 65536);
        assert(width >= 0 && height >= 0);

        UArray2b_T array = malloc(sizeof(*array));
        assert(array != NULL);

        if (width * height * size <= 65536) {
                array->blocksize = 1024;
        } else {
                array->blocksize = 1;
        }

        array->width = width;
        array->height = height;
        array->size = size;

        /*
        - a total of 64KB wil be allocated (this is what it has to fit into)
        - check to make sure that memory was allocated
        - pick a blocksize that is as big as possible so that it fits in 64KB but
                also fits the size of each element
        */

        // 1024 * 64 = 65536 bytes sqrt(65536) = 256.
        
        return array;

}

void UArray2b_free(UArray2b_T *array2b) 
{
        assert(array2b != NULL && *array2b != NULL);

        // free it
}

int UArray2b_width(UArray2b_T array2b)
{
        assert(array2b != NULL);
        return array2b->width;
}

int UArray2b_height(UArray2b_T array2b) 
{
        assert(array2b != NULL);
        return array2b->height;
}

int UArray2b_size(UArray2b_T array2b) 
{
        assert(array2b != NULL);
        return array2b->size;
}

int UArray2b_blocksize(UArray2b_T array2b) 
{
        assert(array2b != NULL);
        return array2b->blocksize;
}

/* return a pointer to the cell in the given column and row.
 * index out of range is a checked run-time error
 */
void *UArray2b_at(UArray2b_T array2b, int column, int row)
{
        assert(array2b != NULL);
        assert(row >= 0 && row < array2b->height);
        assert(column >= 0 && column < array2b->width);
        
        int block_col = column / array2b->blocksize;
        int block_row = row / array2b->blocksize;

        return UArray2_at(array2b->blocks, block_col, block_row); // ? 
}

/* visits every cell in one block before moving to another block */
void UArray2b_map(UArray2b_T array2b,
                          void apply(int col, int row, UArray2b_T array2b,
                                     void *elem, void *pixel),
                          void *pixel)
{
        // if (array2b == NULL || apply == NULL) {
        //         RAISE(Assert_Failed);
        // }
        assert(array2b != NULL);
        assert(apply != NULL);

        printf("Going over these dimensions: %d, %d\n", array2b->blocks_wide, array2b->blocks_high);

        for (int outer_col = 0; outer_col < array2b->blocks_wide; outer_col++) {
                for (int outer_row = 0; outer_row < array2b->blocks_high; outer_row++) {
                        apply(outer_col, outer_row, array2b,
                                UArray2_at(array2b->blocks, outer_col, outer_row),
                                pixel);
                }
        }
}



