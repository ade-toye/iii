/*
 * unblackedges.c
 *
 * Authors: Toye Adebayo & Teny Makuach
 * Date: 2/4/2026
 *
 * Purpose: Removes black edge pixels from a scanned PBM bitmap.
 *          A black edge pixel is any black pixel (value 1) that
 *          is connected to the image border through other black
 *          pixels via 4-connected neighbors. Outputs a plain P1
 *          PBM file with those edge pixels turned white (0).
 *
 * Key Insight: We seed a BFS queue with all black border pixels,
 *          then iteratively spread inward through 4-connected
 *          black neighbors, turning each to white. Using BFS
 *          (not recursive DFS) avoids stack overflow on large
 *          images.
 */

#include <stdlib.h>
#include <stdio.h>
#include "pnmrdr.h"
#include "assert.h"
#include "bit2.h"
#include "mem.h"

/*
 * Queue node storing a single (col, row) coordinate pair.
 */
typedef struct Node {
        int col;
        int row;
        struct Node *next;
} Node;

/*
 * Simple linked-list queue for BFS traversal.
 */
typedef struct Queue {
        Node *front;
        Node *rear;
} Queue;

/*
 * Queue_new
 *
 * Allocates and returns an empty queue. The caller is
 * responsible for freeing it via Queue_free.
 */
static Queue *Queue_new(void)
{
        Queue *q;
        NEW(q);
        q->front = NULL;
        q->rear  = NULL;
        return q;
}

/*
 * Queue_empty
 *
 * Returns nonzero if the queue has no elements, 0 otherwise.
 */
static int Queue_empty(Queue *q)
{
        return q->front == NULL;
}

/*
 * Queue_enqueue
 *
 * Inserts the coordinate (col, row) at the rear of the queue.
 */
static void Queue_enqueue(Queue *q, int col, int row)
{
        Node *n;
        NEW(n);
        n->col  = col;
        n->row  = row;
        n->next = NULL;

        if (q->rear != NULL) {
                q->rear->next = n;
        } else {
                q->front = n;
        }
        q->rear = n;
}

/*
 * Queue_dequeue
 *
 * Removes the front element and stores its coordinates in *col
 * and *row. Frees the removed node.
 *
 * CRE: queue is empty.
 */
static void Queue_dequeue(Queue *q, int *col, int *row)
{
        assert(!Queue_empty(q));

        Node *n = q->front;
        *col = n->col;
        *row = n->row;
        q->front = n->next;

        if (q->front == NULL) {
                q->rear = NULL;
        }
        FREE(n);
}

/*
 * Queue_free
 *
 * Drains any remaining elements and frees the queue struct.
 */
static void Queue_free(Queue *q)
{
        while (!Queue_empty(q)) {
                int c, r;
                Queue_dequeue(q, &c, &r);
        }
        FREE(q);
}

/*
 * enqueue_if_black
 *
 * If (col, row) is within bounds and its bit is black (1),
 * turns it white (0) and enqueues it for BFS processing.
 */
static void enqueue_if_black(Queue *q, Bit2_T bitmap,
                              int col, int row)
{
        int width  = Bit2_width(bitmap);
        int height = Bit2_height(bitmap);

        if (col >= 0 && col < width &&
            row >= 0 && row < height) {
                if (Bit2_get(bitmap, col, row) == 1) {
                        Bit2_put(bitmap, col, row, 0);
                        Queue_enqueue(q, col, row);
                }
        }
}

/*
 * remove_black_edges
 *
 * Performs a BFS from every black border pixel, turning all
 * reachable (4-connected) black pixels to white. Interior
 * black regions not connected to the border are preserved.
 */
static void remove_black_edges(Bit2_T bitmap)
{
        int width  = Bit2_width(bitmap);
        int height = Bit2_height(bitmap);
        Queue *q   = Queue_new();

        /* Seed: black pixels on top and bottom rows */
        for (int col = 0; col < width; col++) {
                enqueue_if_black(q, bitmap, col, 0);
                enqueue_if_black(q, bitmap, col, height - 1);
        }

        /* Seed: black pixels on left and right columns */
        for (int row = 1; row < height - 1; row++) {
                enqueue_if_black(q, bitmap, 0, row);
                enqueue_if_black(q, bitmap, width - 1, row);
        }

        /* BFS: spread to 4-connected neighbors */
        while (!Queue_empty(q)) {
                int col, row;
                Queue_dequeue(q, &col, &row);
                enqueue_if_black(q, bitmap, col - 1, row);
                enqueue_if_black(q, bitmap, col + 1, row);
                enqueue_if_black(q, bitmap, col, row - 1);
                enqueue_if_black(q, bitmap, col, row + 1);
        }

        Queue_free(q);
}

/*
 * print_pbm
 *
 * Writes the bitmap to stdout in plain P1 PBM format.
 * Prints the header line, dimensions, and each row of bits
 * separated by spaces.
 */
static void print_pbm(Bit2_T bitmap)
{
        int width  = Bit2_width(bitmap);
        int height = Bit2_height(bitmap);

        printf("P1\n");
        printf("%d %d\n", width, height);

        for (int row = 0; row < height; row++) {
                for (int col = 0; col < width; col++) {
                        if (col > 0) {
                                printf(" ");
                        }
                        printf("%d", Bit2_get(bitmap, col, row));
                }
                printf("\n");
        }
}

int main(int argc, char *argv[])
{
        FILE *fp = NULL;

        if (argc > 2) {
                fprintf(stderr, "Usage: %s [filename]\n",
                        argv[0]);
                return EXIT_FAILURE;
        } else if (argc == 2) {
                fp = fopen(argv[1], "rb");
                assert(fp != NULL);
        } else {
                fp = stdin;
        }

        Pnmrdr_T reader = Pnmrdr_new(fp);
        Pnmrdr_mapdata data = Pnmrdr_data(reader);

        assert(data.type == Pnmrdr_bit);
        assert(data.width > 0);
        assert(data.height > 0);

        Bit2_T bitmap = Bit2_new((int)data.width,
                                  (int)data.height);

        /* Read all pixels into the Bit2 */
        for (int row = 0; row < (int)data.height; row++) {
                for (int col = 0; col < (int)data.width; col++) {
                        int pixel = Pnmrdr_get(reader);
                        Bit2_put(bitmap, col, row, pixel);
                }
        }

        remove_black_edges(bitmap);
        print_pbm(bitmap);

        /* Clean up all allocated resources */
        Pnmrdr_free(&reader);
        Bit2_free(&bitmap);
        if (fp != stdin) {
                fclose(fp);
        }

        return EXIT_SUCCESS;
}
