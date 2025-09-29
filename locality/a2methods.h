#ifndef A2METHODS_INCLUDED
     2  #define A2METHODS_INCLUDED
     3
     4  #define A2 A2Methods_UArray2
     5
     6  typedef void *A2;               /* unknown type that represents a
     7                                   * 2D array of 'cells'
     8                                   */
     9
    10  typedef void A2Methods_Object;  /* an unknown sequence of bytes in memory
    11                                   * (element of an array)
    12                                   */
    13
    14  typedef void A2Methods_applyfun(int i, int j, A2 array2,
    15                                  A2Methods_Object *ptr, void *cl);
    16  typedef void A2Methods_mapfun(A2 array2, A2Methods_applyfun apply, void *cl);
    17
    18  typedef void A2Methods_smallapplyfun(A2Methods_Object *ptr, void *cl);
    19  typedef void A2Methods_smallmapfun(A2 a2, A2Methods_smallapplyfun f, void *cl);
    20
    21  /* operations on 2D arrays */
    22
    23  /*
    24   * it is a checked run-time error to pass a NULL 2D array to any function,
    25   * and except as noted, a NULL function pointer is an *unchecked* r. e.
    26   */
    27  typedef struct A2Methods_T {
    28          /* creates a distinct 2D array of memory cells,
    29           * each of the given 'size'
    30           *
    31           * each cell is uninitialized
    32           * if the array is blocked, uses a default block size
    33           */
    34          A2(*new)(int width, int height, int size);
    35
    36          /* creates a distinct 2D array of memory cells,
    37           * each of the given 'size'
    38           *
    39           * each cell is uninitialized
    40           * if array is blocked, the block size given is the number of cells
    41           *    along one side of a block; otherwise 'blocksize' is ignored
    42           */
    43          A2(*new_with_blocksize)(int width, int height, int size,
    44                                  int blocksize);
    45
    46          /* frees *array2p and overwrites the pointer with NULL */
    47          void (*free)(A2 *array2p);
    48
    49
    50          /* observe properties of the array */
    51          int (*width)    (A2 array2);
    52          int (*height)   (A2 array2);
    53          int (*size)     (A2 array2);
    54          int (*blocksize)(A2 array2);   /* for unblocked array, returns 1 */
    55
    56          /* returns a pointer to the object in column i, row j
    57           * (checked runtime error if i or j is out of bounds)
    58           */
    59          A2Methods_Object *(*at)(A2 array2, int i, int j);
    60
    61          /* mapping functions */
    62          /* each mapping function visits every cell in array2, and for each
    63           * cell it calls 'apply' with these arguments:
    64           *    i, the column index of the cell
    65           *    j, the row index of the cell
    66           *    array2, the array passed to the mapping function
    67           *    cell, a pointer to the cell
    68           *    cl, the closure pointer passed to the mapping function
    69           *
    70           * These functions differ only in the *order* they visit cells:
    71           *   - row_major visits each row before the next, in order of
    72           *     increasing row index; within a row, column numbers increase
    73           *   - col_major visits each column before the next, in order of
    74           *     increasing column index; within a column, row numbers increase
    75           *   - block_major visits each block before the next; order of
    76           *     blocks and order of cells within a block is not specified
    77           *   - map_default uses a default order that has good locality
    78           *
    79           * In any record, map_block_major may be NULL provided that
    80           * map_row_major and map_col_major are not NULL, and vice versa.
    81           */
    82          void (*map_row_major)(A2 array2, A2Methods_applyfun apply, void *cl);
    83          void (*map_col_major)(A2 array2, A2Methods_applyfun apply, void *cl);
    84          void (*map_block_major)(A2 array2, A2Methods_applyfun apply,
    85                                  void *cl);
    86          void (*map_default)(A2 array2, A2Methods_applyfun apply, void *cl);
    87
    88          /*
    89           * alternative mapping functions that pass only
    90           * cell pointer and closure
    91           */
    92          void (*small_map_row_major)  (A2 a2, A2Methods_smallapplyfun apply,
    93                                        void *cl);
    94          void (*small_map_col_major)  (A2 a2, A2Methods_smallapplyfun apply,
    95                                        void *cl);
    96          void (*small_map_block_major)(A2 a2, A2Methods_smallapplyfun apply,
    97                                        void *cl);
    98          void (*small_map_default)    (A2 a2, A2Methods_smallapplyfun apply,
    99                                        void *cl);
   100
   101  } *A2Methods_T;
   102
   103  #undef A2
   104
   105  #endif
