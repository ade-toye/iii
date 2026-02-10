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
 * Allocates and returns a new empty queue. The front and rear
 * pointers are initialized to NULL. The caller is responsible
 * for freeing the queue via Queue_free.
 *
 * CRE: memory allocation failure.
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
 * Returns 1 if the queue has no elements (front is NULL),
 * and 0 if the queue has at least one element.
 *
 * CRE: None.
 */
static int Queue_empty(Queue *q)
{
        return q->front == NULL;
}

/*
 * Queue_enqueue
 *
 * Adds a new node containing the coordinates (col, row) to
 * the rear (back) of the queue. If the queue is empty, the
 * new node becomes both front and rear. Otherwise it is
 * appended to the existing rear.
 *
 * CRE: memory allocation failure.
 */
static void Queue_enqueue(Queue *q, int col, int row)
{
        Node *n;
        NEW(n);
        n->col  = col;
        n->row  = row;
        n->next = NULL;

        if (q->rear != NULL) {
                q->rear->next = n;   /* Add to end if exists */
        } else {
                q->front = n;        /* Queue was empty */
        }
        q->rear = n;
}

/*
 * Queue_dequeue
 *
 * Removes the front element from the queue and stores its
 * col and row coordinates in the pointers provided. The
 * removed node is freed. If the queue becomes empty after
 * removal, rear is set to NULL.
 *
 * CRE: queue is empty (asserts on this condition).
 */
static void Queue_dequeue(Queue *q, int *col, int *row)
{
        assert(!Queue_empty(q));

        Node *n = q->front;
        *col = n->col;              /* Extract coordinates */
        *row = n->row;
        q->front = n->next;

        if (q->front == NULL) {
                q->rear = NULL;     /* Queue is now empty */
        }
        FREE(n);
}

/*
 * Queue_free
 *
 * Removes and frees all remaining nodes in the queue by
 * repeatedly calling Queue_dequeue. Then frees the queue
 * struct itself.
 *
 * CRE: None.
 */
static void Queue_free(Queue *q)
{
        while (!Queue_empty(q)) {
                int c, r;
                Queue_dequeue(q, &c, &r); /* Remove and free nodes */
        }
        FREE(q);
}

/*
 * enqueue_if_black
 *
 * Checks if (col, row) is within the bitmap bounds and if
 * the pixel at that location is black (value 1). If both
 * conditions are true, sets the pixel to white (0) and
 * enqueues the coordinates in the queue.
 *
 * CRE: None.
 */
static void enqueue_if_black(Queue *q, Bit2_T bitmap,
                              int col, int row)
{
        int width  = Bit2_width(bitmap);
        int height = Bit2_height(bitmap);

        /* Check if coordinates are valid */
        if (col >= 0 && col < width &&
            row >= 0 && row < height) {
                /* Check if pixel is black */
                if (Bit2_get(bitmap, col, row) == 1) {
                        Bit2_put(bitmap, col, row, 0);
                        Queue_enqueue(q, col, row);
                }
        }
}

/*
 * remove_black_edges
 *
 * Performs a breadth-first search (BFS) starting from all
 * black pixels on the bitmap border. For each black pixel
 * found, it is turned white and its 4-connected neighbors
 * (up, down, left, right) are checked. This removes all
 * black pixels connected to the edge. Interior black regions
 * not touching the border are preserved.
 *
 * CRE: bitmap dimensions are <= 0.
 */
static void remove_black_edges(Bit2_T bitmap)
{
        int width  = Bit2_width(bitmap);
        int height = Bit2_height(bitmap);
        Queue *q   = Queue_new();

        /* Add black pixels from all four edges to queue */
        for (int col = 0; col < width; col++) {
                enqueue_if_black(q, bitmap, col, 0);
                enqueue_if_black(q, bitmap, col, height - 1);
        }
        for (int row = 1; row < height - 1; row++) {
                enqueue_if_black(q, bitmap, 0, row);
                enqueue_if_black(q, bitmap, width - 1, row);
        }

        /* Spread to neighbors left, right, up, down */
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
 * Writes the bitmap to standard output in P1 PBM format.
 * The output begins with "P1" (plain PBM magic number),
 * followed by width and height on one line, then each row
 * of bits separated by single spaces, with rows on separate
 * lines.
 *
 * CRE: None.
 */
static void print_pbm(Bit2_T bitmap)
{
        int width  = Bit2_width(bitmap);
        int height = Bit2_height(bitmap);

        printf("P1\n");             /* P1 is plain PBM format */
        printf("%d %d\n", width, height);

        for (int row = 0; row < height; row++) {
                for (int col = 0; col < width; col++) {
                        if (col > 0) {
                                printf(" "); /* Space between bits */
                        }
                        printf("%d", Bit2_get(bitmap, col, row));
                }
                printf("\n");
        }
}

/*
 * main
 *
 * Reads a PBM image from a file (if provided as argument)
 * or from standard input. Creates a bitmap and loads all
 * pixels. Calls remove_black_edges to remove edge pixels,
 * then prints the result in P1 PBM format. Accepts 0 or 1
 * command-line argument. With 0 args uses stdin, with 1 arg
 * opens that file.
 *
 * CRE: more than 1 command-line argument provided.
 * CRE: file cannot be opened for reading.
 * CRE: input is not a valid PBM image.
 */
int main(int argc, char *argv[])
{
        FILE *fp = NULL;

        /* Open file for reading if provided, else use stdin */
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

        /* Create reader and check image is valid PBM */
        Pnmrdr_T reader = Pnmrdr_new(fp);
        Pnmrdr_mapdata data = Pnmrdr_data(reader);

        assert(data.type == Pnmrdr_bit);
        assert(data.width > 0);
        assert(data.height > 0);

        /* Create bitmap to hold all pixels */
        Bit2_T bitmap = Bit2_new((int)data.width,
                                  (int)data.height);

        /* Read and store all pixels from input */
        for (int row = 0; row < (int)data.height; row++) {
                for (int col = 0; col < (int)data.width; col++) {
                        int pixel = Pnmrdr_get(reader);
                        Bit2_put(bitmap, col, row, pixel);
                }
        }

        /* Process image and output */
        remove_black_edges(bitmap);
        print_pbm(bitmap);

        /* Free all memory and close files */
        Pnmrdr_free(&reader);
        Bit2_free(&bitmap);
        if (fp != stdin) {
                fclose(fp);
        }

        return EXIT_SUCCESS;
}