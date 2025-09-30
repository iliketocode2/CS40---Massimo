/**************************************************************
 *
 *                     urray2.c
 *
 *     Assignment: iii
 *     Authors:  William Goldman (wgoldm03) and Andrew Bacigalupi (abacig01) 
 *     Date:     September 24, 2025
 *
 *     This is the implementation for a 2D unboxed array (UArray2) module.
 *
 *
 **************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "uarray2.h"
#include "uarray.h"
#include "except.h"

const Except_T Assert_Failed = { "Assertion failed" };
const Except_T Memory_Error = { "Memory allocation failed" };
const Except_T Out_Of_Bounds = { "Index out of bounds" };

struct UArray2_T {
    int width;
    int height;
    UArray_T grid;
};

/********** UArray2_new ********
 *
 * Create a new UArray2_T with width columns and height rows. Each element
 * is of the specified size.
 *
 * Parameters:
 *      width:  int number of columns in the new array
 *      height: int number of rows in the new array
 *      size:   int size of each element in the array in bytes
 *
 * Return: void
 *
 * Expects
 *      width and height to be > 0 and size > 0 and arr != NULL
 * Notes:
 *      Will CRE if above expectations are not met.
 *      This function allocates heap memory without freeing, so it must 
 *      be freed by the user via UArray2_free()
 ************************/
UArray2_T UArray2_new(int width, int height, int size) 
{  
        UArray2_T instance = malloc(sizeof(*instance));
        if (instance == NULL) {
                RAISE(Memory_Error);
        }

        if (width < 0 || height < 0 || size < 1) {
                RAISE(Assert_Failed);
        }

        instance->width = width;
        instance->height = height;
        instance->grid = UArray_new(height * width, size);
        return instance;
}

/********** UArray2_free ********
 *
 * Free the memory associated with a UArray2_T instance
 *
 * Parameters:
 *      arr: pointer to UArray2_T
 *
 * Return: void
 *
 * Expects
 *      arr != NULL and *arr != NULL
 * Notes:
 *      Will CRE if arr == NULL or *arr == NULL
 ************************/
void UArray2_free(UArray2_T *arr) 
{
        if (arr == NULL || *arr == NULL) {
                RAISE(Assert_Failed);
        }

        UArray_free(&((*arr)->grid));
        free(*arr);
}

/********** UArray2_height ********
 *
 * Retrieve the height (number of rows) of the 2D array.
 *
 * Parameters:
 *     arr: pointer to UArray2_T
 *
 * Return: int height of the array
 *
 * Expects
 *      arr != NULL
 * Notes:
 *      Will CRE if arr == NULL
 ************************/
int UArray2_height(UArray2_T arr) 
{
        if (arr == NULL) {
               RAISE(Assert_Failed); 
        }
        return arr->height;
}

/********** UArray2_width ********
 *
 * Retrieve the width (number of columns) of the 2D array.
 *
 * Parameters:
 *     arr: pointer to UArray2_T
 * 
 * Return: int width of the array
 *
 * Expects
 *      arr != NULL
 * Notes:
 *      Will CRE if arr == NULL
 ************************/
int UArray2_width(UArray2_T arr) 
{
        if (arr == NULL) {
               RAISE(Assert_Failed); 
        }
        return arr->width;
}

/********** UArray2_size ********
 *
 * Retrieve the size of each element in the 2D array.
 *
 * Parameters:
 *     arr: pointer to UArray2_T
 *
 * Return: int size of each element in the array
 *
 * Expects
 *      arr != NULL
 * Notes:
 *      Will CRE if arr == NULL
 ************************/
int UArray2_size(UArray2_T arr) 
{
        if (arr == NULL) {
               RAISE(Assert_Failed); 
        }
        return UArray_size(arr->grid);
}

/********** UArray2_at ********
 *
 * Access the element in the 2D array at the specified column and row.
 * 
 * Parameters:
 *      arr:           pointer to UArray2_T
 *      int col:       the column index, or the number of columns to the left 
 *                     of it in the 2D array
 *      int row:       the row index, or the number of rows above it in the 2D 
 *                     array
 *
 * Return: void pointer to the element that is accessed
 *
 * Expects
 *      col <= width - 1 and row <= height - 1
        
 * Notes:
 *      Will CRE if arr == NULL or col > width - 1 or row > height - 1
 ************************/
void *UArray2_at(UArray2_T arr, int col, int row) 
{
        if (arr == NULL) {
                RAISE(Assert_Failed);
        }
        if (col < 0 || col >= arr->width || row < 0 || row >= arr->height) {
                RAISE(Out_Of_Bounds);
        }
        return UArray_at(arr->grid, (row * arr->width) + col);
}
        
/********** UArray2_map_row_major ********
 *
 * Apply a function to each element in the 2D array in row-major order.
 * 
 * Parameters:
 *     arr:            UArray2_T
 *     (void)(*apply): function pointer with parameters (int row, int col,
 *                     UArray2_T arr, void *p1, void *p2)
 *     cl:             closure pointer (used to track state in any form)
 *
 * Return: void
 *
 * Expects
 *      arr != NULL, apply != NULL
 * Notes:
 *      Will CRE if above conditions not met
 ************************/
void UArray2_map_row_major(UArray2_T arr, void apply(int col, int row, 
                                        UArray2_T arr, void *elem, void *cl), 
                                        void *cl)
{
        if (arr == NULL || apply == NULL) {
                RAISE(Assert_Failed);
        }

        for (int row = 0; row < arr->height; row++) {
                for (int col = 0; col < arr->width; col++) {
                        apply(col, row, arr, UArray2_at(arr, col, row), cl);
                }
        }
}

/********** UArray2_map_col_major ********
 *
 * Apply a function to each element in the 2D array in col-major order.
 * 
 * Parameters:
 *     arr:            UArray2_T
 *.    (void)(*apply): function pointer with parameters (int row, int col,
 *                     UArray2_T a, void *p1, void *p2)
 *     void *cl:             closure pointer (used to track state in any form)
 *
 * Return: int size of each element in the array
 *
 * Expects
 *      arr != NULL, apply != NULL
 *
 * Notes:
 *      Will CRE if above conditions are not met
 ************************/
void UArray2_map_col_major(UArray2_T arr, void apply(int col, int row, 
                                        UArray2_T arr, void *elem, void *cl), 
                                        void *cl)
{
        if (arr == NULL || apply == NULL) {
                RAISE(Assert_Failed);
        }

        for (int col = 0; col < arr->width; col++) {
                for (int row = 0; row < arr->height; row++) {
                        apply(col, row, arr, UArray2_at(arr, col, row), cl);
                }
        }
}