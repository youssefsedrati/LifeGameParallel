#ifndef RUN_OMP_H
#define RUN_OMP_H

#include <omp.h>

typedef struct Board Board;

/**
 * Runs a given number of iterations of the game of life on a board
 * OMP version.
 *
 * @param b             An initialized board
 * @param nbIterations  Number of iterations to run
 * @param nbThreads     Number of threads to use
 */
extern void run_OMP(Board *b, int nbIterations, int nbThreads);

#endif //RUN_OMP_H
