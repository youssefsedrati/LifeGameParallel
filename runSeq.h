#ifndef RUN_REF_H
#define RUN_REF_H

typedef struct Board Board;

/**
 * Runs a given number of iterations of the game of life on a board
 * Reference sequential version.
 *
 * @param b An initialized board
 * @param nbIterations The number of iterations to run
 */
extern void run_seq(Board *b, int nbIterations);

#endif //RUN_REF_H