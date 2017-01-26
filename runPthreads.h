#ifndef RUN_PTHREADS_H
#define RUN_PTHREADS_H

typedef struct Board Board;

/**
 * Runs a given number of iterations of the game of life on a board
 * Pthreads version.
 *
 * @param b An initialized board
 * @param nbIterations Number of iterations to run
 * @param nbThreads Number of threads to use
 */
extern void run_pthreads(Board *b, int nbIterations, int nbThreads);


#endif //RUN_PTHREADS_H
