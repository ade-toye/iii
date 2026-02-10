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
 * name: Queue_new
 *
 * description: Creates and returns a new empty queue. The queue
 * has no elements when first created. The caller is responsible
 * for freeing the queue by calling Queue_free.
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   A pointer to a newly allocated empty queue
 *
 * CRE: Memory allocation fails (program will crash).
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
 * name: Queue_empty
 *
 * description: Checks if the queue is empty. Returns 1 if empty,
 * 0 if the queue has at least one element.
 *
 * Parameters:
 *   q - pointer to the queue to check
 *
 * Returns:
 *   1 if the queue is empty, 0 if it has elements
 *
 * CRE: q is NULL.
 */
static int Queue_empty(Queue *q)
{
        return q->front == NULL;
}

/*
 * name: Queue_enqueue
 *
 * description: Adds a new position (col, row) to the back of
 * the queue. If the queue is empty, this becomes the first
 * element. Otherwise it is added to the end.
 *
 * Parameters:
 *   q   - pointer to the queue
 *   col - column coordinate to add
 *   row - row coordinate to add
 *
 * Returns:
 *   void
 *
 * CRE: q is NULL.
 * CRE: Memory allocation fails (program will crash).
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
 * name: Queue_dequeue
 *
 * description: Removes and returns the first element from the
 * queue. The coordinates are stored in the pointers provided.
 * The removed element is freed from memory.
 *
 * Parameters:
 *   q   - pointer to the queue
 *   col - pointer to store the column coordinate
 *   row - pointer to store the row coordinate
 *
 * Returns:
 *   void
 *
 * CRE: q is NULL.
 * CRE: The queue is empty.
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
 * name: Queue_free
 *
 * description: Frees all memory used by the queue. All remaining
 * elements are removed and deleted, then the queue structure
 * itself is deleted.
 *
 * Parameters:
 *   q - pointer to the queue to free
 *
 * Returns:
 *   void
 *
 * CRE: q is NULL.
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
 * name: enqueue_if_black
 *
 * description: Checks if a pixel at the given position is black.
 * If the position is in bounds and the pixel is black (value 1),
 * it changes the pixel to white (0) and adds the position to
 * the queue.
 *
 * Parameters:
 *   q      - pointer to the queue
 *   bitmap - the bitmap image
 *   col    - column coordinate to check
 *   row    - row coordinate to check
 *
 * Returns:
 *   void
 *
 * CRE: q is NULL.
 * CRE: bitmap is NULL.
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
 * name: remove_black_edges
 *
 * description: Removes black pixels that connect to the image
 * border. Starting from black pixels on all four edges, it
 * spreads inward through neighbors that are also black, turning
 * each to white. Black regions completely surrounded by white
 * pixels are left unchanged.
 *
 * Parameters:
 *   bitmap - the bitmap image to process
 *
 * Returns:
 *   void
 *
 * CRE: bitmap is NULL.
 * CRE: bitmap width or height is less than 1.
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
 * name: print_pbm
 *
 * description: Outputs the bitmap to the screen in plain PBM
 * format (P1). Each row of pixels is printed on its own line,
 * with pixel values separated by spaces.
 *
 * Parameters:
 *   bitmap - the bitmap image to print
 *
 * Returns:
 *   void
 *
 * CRE: bitmap is NULL.
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
 * name: main
 *
 * description: Reads a bitmap image and removes black pixels
 * connected to the image borders. The image is read from a file
 * (if a filename is provided) or from standard input. After
 * processing, the result is printed as a plain PBM image.
 *
 * Parameters:
 *   argc - number of command-line arguments (must be 1 or 2)
 *   argv - array of command-line arguments (optional filename)
 *
 * Returns:
 *   EXIT_SUCCESS if image is processed successfully,
 *   EXIT_FAILURE if arguments are invalid
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