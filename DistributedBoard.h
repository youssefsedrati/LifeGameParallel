#ifndef DISTRIBUTED_BOARD_H
#define DISTRIBUTED_BOARD_H

#include "Board.h"

typedef struct Board Board;

typedef struct DistributedBoard
{
    int N;           // Size of the distributed board (NxN)
    int Nx;          // Width of each slice (that is, of each local board exluding ghost cells)
    int nbNodes;
    int nodeRank;
    Board *b;
} DistributedBoard;

/**
 * Initialize an empty distributed board of specified size
 *
 * @param b         Pointer on an uninitialized distributed board
 * @param N         Size of the board
 */
extern void init_distributed_board(DistributedBoard *db, int N);

/**
 * Set the state of a column of cells
 * @param b         Pointer on an initialized distributed board
 * @param x         X-coordinate of the column
 * @param val       Value to set
 */
extern void set_distributed_col(DistributedBoard *db, int x, int val);

/**
 * Set the state of a row of cells
 * @param b         Pointer on an initialized distributed board
 * @param y         Y-coordinate of the row
 * @param val       Value to set
 */
extern void set_distributed_row(DistributedBoard *db, int y, int val);

/**
 * Release the resources associated with a distributed board.
 * @param b An initialized distributed board
 */
extern void free_distributed_board(DistributedBoard *db);

#endif //DISTRIBUTED_BOARD_H
