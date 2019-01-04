/* edge_removal.c
 * Sam Swoap & Sammy Stolzenbach
 * 2/12/18
 * This file unblackens the edges of a pbm
 */

#include <stdio.h>
#include <stdlib.h>
#include <seq.h>
#include <assert.h>
#include <pnmrdr.h>
#include "bit2.h"

typedef struct Pixel Pixel;
struct Pixel {

        int row;
        int col;
};

Bit2_T pbmread(FILE *fp);
void unblacken(Bit2_T bitmap);
void pbmwrite(FILE *fp, Bit2_T bitmap);
void store_pixel(int row, int col, Bit2_T bitmap, int element, void * cl);
void print_pixel(int row, int col, Bit2_T bitmap, int element, void * cl);
void store_outer_layer(Bit2_T bitmap, Seq_T blackedges);
void add_to_queue(int row, int col, Seq_T blackedges);
void check_neighbors(Pixel *pixel, Seq_T blackedges, Bit2_T bitmap);
void remove_black_edges(Bit2_T bitmap, Seq_T blackedges);


/* ERRORS: CRE if there are more than 2 args 
           CRE if file can't be opened 
*/
int main(int argc, char const *argv[])
{
        FILE *fp;
        Bit2_T bitmap;
        assert(argc <=2);
        if(argc == 2) {
                fp = fopen(argv[1], "rb");
                assert(fp != NULL);
        }
        else
                fp = stdin;

        bitmap = pbmread(fp);
        unblacken(bitmap);
        pbmwrite(stdout, bitmap);

        Bit2_free(&bitmap);
        fclose(fp);

        return 0;
}

/* ARGS: FILE pointer
 * DOES: Reads pbm data into a bit2
 * RETURNS: bitmap with data from pbm
 * ERRORS: CRE if file type is not a pbm
 */
Bit2_T pbmread(FILE *fp)
{
        Pnmrdr_T reader = Pnmrdr_new(fp);
        Pnmrdr_mapdata mapdata = Pnmrdr_data(reader);
        assert(mapdata.type == Pnmrdr_bit);
        Bit2_T bitmap = Bit2_new(mapdata.width, mapdata.height);

        Bit2_map_row_major(bitmap, store_pixel, reader);
        Pnmrdr_free(&reader);
        return bitmap;
}

/* ARGS: FIlE pointer, a bit2
 * DOES: Writes content of bit2 to stdout
 * RETURNS: nothing
 * ERRORS: None
 */
void pbmwrite(FILE *fp, Bit2_T bitmap)
{
        fprintf(fp, "P1\n%d %d\n", Bit2_width(bitmap), Bit2_height(bitmap));
        Bit2_map_row_major(bitmap, print_pixel, stdout);

}

/* ARGS: a row, a col, a Bit2, an element in Bit2, 
 *  a closure(the pnmrdr reader)
 * DOES: Stores a pixel in the Bit2
 * RETURNS: nothing
 * ERRORS: None
 */
void store_pixel(int row, int col, Bit2_T bitmap, int element, void * cl)
{
        Pnmrdr_T reader = (Pnmrdr_T) cl;
        Bit2_put(bitmap, row, col, Pnmrdr_get(reader));
        (void) element;
}

/* ARGS: a row, a col, a Bit2, an element in the Bit2, 
    a closure (FILE pointer)
 * DOES: Prints a single pixel to stdout
 * RETURNS: nothing
 * ERRORS: None
 */
void print_pixel(int row, int col, Bit2_T bitmap, int element, void * cl)
{
        FILE *fp = (FILE *)cl;
        if (col == Bit2_width(bitmap) -1) {
                fprintf(fp, "%d", element); 
                if(row != Bit2_height(bitmap) - 1)
                        fprintf(fp, "\n"); 
        }
        else
                fprintf(fp, "%d ", element);
}

/* ARGS: a Bit2
 * DOES: unblackens the edges
 * RETURNS: nothing
 * ERRORS: None 
 */
void unblacken(Bit2_T bitmap)
{       
       Seq_T blackedges = Seq_new(Bit2_width(bitmap) * Bit2_height(bitmap));
       store_outer_layer(bitmap,  blackedges);
       remove_black_edges(bitmap, blackedges);

       Seq_free(&blackedges);
}

/* ARGS: A Bit2, a Seq
 * DOES: Store black border pixels in blackedges Seq
 * RETURNS: nothing
 * ERRORS: None
 */
void store_outer_layer(Bit2_T bitmap, Seq_T blackedges)
{
        int row, col, width, height;
        height = Bit2_height(bitmap);
        width = Bit2_width(bitmap);

        for (row = 0; row < height; ++row) {
                if (Bit2_get(bitmap, row, 0) == 1)
                        add_to_queue(row, 0, blackedges);
                if (Bit2_get(bitmap, row, width - 1) == 1)
                        add_to_queue(row, width - 1, blackedges);
        }

        for (col = 0; col < width; ++col) {
                if (Bit2_get(bitmap, 0, col) == 1)
                        add_to_queue(0, col, blackedges);
                if (Bit2_get(bitmap, height - 1, col) == 1)
                        add_to_queue(height - 1, col, blackedges);
        }
}

/* ARGS: a row, a col, a Seq
 * DOES: Adds a pixel struct to the blackedges Seq
 * RETURNS: nothing
 * ERRORS: CRE if malloc fails
 */
void add_to_queue(int row, int col, Seq_T blackedges)
{
        Pixel *pixel = malloc(sizeof(*pixel));
        assert(pixel != NULL);

        pixel->row = row;
        pixel->col = col;
        Seq_addhi(blackedges, pixel);
}

/* ARGS: a Bit2, a Seq
 * DOES: changes each black edge pixels to white
 * RETURNS: nothing
 * ERRORS: none
 */
void remove_black_edges(Bit2_T bitmap, Seq_T blackedges)
{
        while(Seq_length(blackedges) != 0) {
                Pixel *pixel = (Pixel *)Seq_remlo(blackedges);

                if(Bit2_get(bitmap, pixel->row, pixel->col) == 0) {
                        free(pixel);
                        continue;
                }
                Bit2_put(bitmap, pixel->row, pixel->col, 0);
                check_neighbors(pixel, blackedges, bitmap);
                free(pixel);
        }
}

/* ARGS: Pointer to a Pixel struct, a Seq, a Bit2
 * DOES: adds neighboring pixels that are black to Seq
 * RETURNS: nothing
 * ERRORS: None
 */
void check_neighbors(Pixel *pixel, Seq_T blackedges, Bit2_T bitmap)
{
        int row = pixel->row, col = pixel->col;
        int height = Bit2_height(bitmap), width = Bit2_width(bitmap);

        if (row != 0)
                if (Bit2_get(bitmap, row - 1, col) == 1)
                        add_to_queue(row - 1, col, blackedges);

        if (row != height - 1)
                if (Bit2_get(bitmap, row + 1, col) == 1)
                        add_to_queue(row + 1, col, blackedges);

        if (col != 0)
                if (Bit2_get(bitmap, row, col - 1) == 1)
                        add_to_queue(row, col - 1, blackedges);

        if (col != width - 1)
                if (Bit2_get(bitmap, row, col + 1) == 1)
                        add_to_queue(row, col + 1, blackedges);
}