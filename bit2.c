/* bit2.c
 * Sam Swoap & Sammy Stolzenbach
 * 2/12/18
 * This file is the implentation of Bit2
 */
#include <stdio.h>
#include <stdlib.h>
#include "bit2.h"
#include "bit.h"
#include "assert.h"

#define T Bit2_T

struct T {

        Bit_T base_bit;
        int width;
        int height;
};


int index(Bit2_T barray, int row, int col);

/* ARGS: a width, a height
 * DOES: creates a new Bit2 with those dimensions
 * RETURNS: a pointer to that Bit2
 * ERRORS: CRE if width * height is less than zero
 *         CRE if malloc fails, 
 */
Bit2_T Bit2_new(int width, int height)
{
        int length = height * width;

        assert(length >= 0);

        Bit2_T bit_ptr = malloc(sizeof(*bit_ptr));
        assert(bit_ptr != NULL);

        bit_ptr->height = height;
        bit_ptr->width  =  width;

        bit_ptr->base_bit = Bit_new(length);
        return bit_ptr;
}


/* ARGS: a Bit2_T, a row and col
 * DOES: computes the resulting index in the Bit2
 * RETURNS: that index
 * ERRORS: CRE if row or col is out of range.
 */
int index(Bit2_T barray, int row, int col)
{
        assert(row < Bit2_height(barray));
        assert(col < Bit2_width(barray));
        return (row * barray->width) + col;
}


/* ARGS: a Bit2_T, a row and col, and an input
 * DOES: puts the input (0 or 1) in row, col
 * RETURNS: the value stored in row, col prior to function call
 * ERRORS: CRE if row or col is out of range.
 */
int Bit2_put(Bit2_T barray, int row, int col, int input)
{
        int loc = index(barray, row, col);
        return Bit_put(barray->base_bit, loc, input);
}

/* ARGS: a Bit2_T, a row and col
 * DOES: returns the input (0 or 1) in row, col
 * RETURNS: the value stored in row, col
 * ERRORS: if the indices are out of range
 */
int Bit2_get(Bit2_T barray, int row, int col)
{
        int loc = index(barray, row, col);
        return Bit_get(barray->base_bit, loc);
}


/* ARGS: Bit2_T
 * DOES: gets the width
 * RETURNS: the width
 */
int Bit2_width(Bit2_T barray)
{
        return barray->width;
}

/* ARGS: Bit2T
 * DOES: gets the height
 * RETURNS: the height
 */
int Bit2_height(Bit2_T barray)
{
        return barray->height;
}

/* ARGS: a pointer to a Bit2_T
 * DOES: free the memory associated with the Bit2_T
 * RETURNS: None
 */
void Bit2_free(Bit2_T *barray)
{
        Bit_free(&(*barray)->base_bit);
        free(*barray);
        *barray = NULL;
}


/* ARGS: a Bit2_T, an apply() function, a pointer to clojure 
 * DOES: applys that function to every element in the Bit2, 
 * varying cols more quickly
 * RETURNS: None
 */
void Bit2_map_row_major(Bit2_T barray, 
        void apply(int row, int col, Bit2_T, 
        int element, void * cl), void *cl)
{
        int row, col;
        for (row = 0; row < barray->height; ++row)
                for (col = 0; col < barray->width; ++col) 
                        apply(row, col, barray, Bit2_get(barray, row, col), cl);
}
        

/* ARGS: a Bit2_T, an apply() function, a pointer to clojure 
 * DOES: applys that function to every element in the Bit2, 
 * varying rows more quickly
 * RETURNS: None
 */
void Bit2_map_col_major(Bit2_T barray, 
        void apply(int row, int col, Bit2_T, 
        int element, void * cl), void *cl)
{
        int row, col;
        for (col = 0; col < barray->width; ++col)
                for (row = 0; row < barray->height; ++row) 
                        apply(row, col, barray, Bit2_get(barray, row, col), cl);

}
