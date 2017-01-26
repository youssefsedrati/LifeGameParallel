#include "DistributedBoard.h"

#include <assert.h>
#include <mpi.h>
#include <malloc.h>

void init_distributed_board(DistributedBoard *db, int N)
{
    MPI_Comm_size(MPI_COMM_WORLD, &(db->nbNodes));
    MPI_Comm_rank(MPI_COMM_WORLD, &(db->nodeRank));
    db->N = N;
    db->Nx = N / db->nbNodes;
    db->b = (Board *) malloc(sizeof(Board));
    init_board(db->b, db->Nx, N);
}

void set_distributed_row(DistributedBoard *db, int y, int val)
{
    set_row(db->b, y, val);
}

void set_distributed_col(DistributedBoard *db, int x, int val)
{
    int concernedRank = x / db->Nx;
    int prevRank = (concernedRank == 0) ? db->nbNodes - 1 : concernedRank - 1;
    int nextRank = (concernedRank == db->nbNodes - 1) ? 0 : concernedRank + 1;
    int localX = x - concernedRank * db->Nx;

    if (db->nodeRank == concernedRank)
        set_col(db->b, localX, val);

    if (localX == 0 && db->nodeRank == prevRank)
        set_col(db->b, db->Nx, val);

    if (localX == db->Nx-1 && db->nodeRank == nextRank)
        set_col(db->b, -1, val);
}

void free_distributed_board(DistributedBoard *db)
{
    free_board(db->b);
    free(db->b);
}
