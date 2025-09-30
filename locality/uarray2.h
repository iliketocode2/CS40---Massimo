
/**************************************************************
 *
 *                     urray2.h
 *
 *     Assignment: iii
 *     Authors:  William Goldman (wgoldm03) and Andrew Bacigalupi (abacig01) 
 *     Date:     September 24, 2025
 *
 *     This is the interface for a 2D unboxed array (UArray2) module.
 *
 *
 **************************************************************/

#ifndef UARRAY2_H 
#define UARRAY2_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "except.h"

#define T UArray2_T
typedef struct T *T;

extern T UArray2_new(int width, int height, int size);
extern void UArray2_free(T *arr);
extern void *UArray2_at(T arr, int col, int row);
extern int UArray2_size(T arr);
extern int UArray2_width(T arr);
extern int UArray2_height(T arr);
extern void UArray2_map_row_major(T arr, void apply(int col, int row, 
                                        T arr, void *elem, void *cl), 
                                        void *cl);
extern void UArray2_map_col_major(T arr, void apply(int col, int row, 
                                        T arr, void *elem, void *cl), 
                                        void *cl);

#undef T
#endif