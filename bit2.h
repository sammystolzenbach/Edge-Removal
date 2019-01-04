/* 
 * bit2.h
 * Sam Swoap & Sammy Stolzenbach
 * 2/2/18
 * This file defines the interface of our bit2
 */
#ifndef BIT2_INCLUDED
#define BIT2_INCLUDED

#include <stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include <bit.h>

#define T Bit2_T
typedef struct T *T;


/* ARGS: a width, a height, a size
 * DOES: creates a new Bit2 with those dimensions
 * RETURNS: a pointer to that Bit2
 * ERRORS: CRE if width * height is less than zero
 *         CRE if malloc fails, 
 */
T Bit2_new(int height, int width);

/* ARGS: a Bit2_T, a row, a col, a value (0 or 1)
 * DOES: Stores the value given in row,col
 * RETURNS: the previously stored value in row,col
 */
int Bit2_put(T barray, int row, int col, int n);


/* ARGS: a Bit2_T, a row, a col
 * DOES: returns the value in row,col
 * RETURNS: the stored value in row,col
 */
int Bit2_get(T barray, int row, int col);

/* ARGS: Bit2T
 * DOES: gets the width
 * RETURNS: the width
 */
int Bit2_width(T barray);

/* ARGS: Bit2T
 * DOES: gets the height
 * RETURNS: the height
 */
int Bit2_height(T barray);

/* ARGS: a pointer to a barray_T
 * DOES: free the memory associated with that array, and sets to NULL
 * RETURNS: None
 */
void Bit2_free(T *barray);


/* ARGS: a Bit2_T, an apply() function, a pointer to clojure 
 * DOES: applys that function to every element in the array, varying cols
 * more quickly
 * RETURNS: None
 */
void Bit2_map_row_major(T barray, 
        void apply(int row, int col, T, 
        int element, void * cl), void *cl);
        

/* ARGS: a Bit2_T, an apply() function, a pointer to clojure 
 * DOES: applys that function to every element in the array, varying rows 
 * more quickly
 * RETURNS: None
 */
void Bit2_map_col_major(T barray, 
        void apply(int row, int col, T, 
        int element, void * cl), void *cl);

#undef T
#endif
