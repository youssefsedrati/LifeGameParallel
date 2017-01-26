#include "runMpiSeq.h"
#include "Board.h"
#include "DistributedBoard.h"

#include <mpi.h>

#define SHIFT_LEFT_EVEN 42
#define SHIFT_RIGHT_EVEN 43
#define SHIFT_LEFT_ODD 44
#define SHIFT_RIGHT_ODD 45

void run_MPI_seq_sync(DistributedBoard *db, int nbIterations)
{
    int leftNodeRank = (db->nodeRank== 0) ? db->nbNodes - 1 : db->nodeRank - 1;
    int rightNodeRank = (db->nodeRank == db->nbNodes - 1) ? 0 : db->nodeRank + 1;
    Board *b = db->b;

    for (int iter = 0; iter < nbIterations; iter++)
    {
        // Count neighbours
        count_neighbours_block_col(b, 0, b->Nx);

        // Update cells
        for (int x = 0; x < b->Nx; x++)
            update_state_col(b, x);

        MPI_Sendrecv(&(b->arr[(b->Nx - 1) * b->ld - 1]), b->ld, MPI_FLOAT, rightNodeRank, 42,
                     &(b->arr[- 1 * b->ld - 1]), b->ld, MPI_FLOAT, leftNodeRank, 42, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Sendrecv(&(b->arr[0 * b->ld - 1]), b->ld, MPI_FLOAT, leftNodeRank, 43,
                     &(b->arr[b->Nx * b->ld - 1]), b->ld, MPI_FLOAT, rightNodeRank, 43, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
}


void run_MPI_seq_async(DistributedBoard *db, int nbIterations)
{
    MPI_Request sendLeft;
    MPI_Request sendRight;
    MPI_Request recvLeft;
    MPI_Request recvRight;

    int leftNodeRank = (db->nodeRank== 0) ? db->nbNodes - 1 : db->nodeRank - 1;
    int rightNodeRank = (db->nodeRank == db->nbNodes - 1) ? 0 : db->nodeRank + 1;
    Board *b = db->b;

    for (int iter = 0; iter < nbIterations; iter++)
    {
        // Update borders
        count_neighbours_col(b, 0);
        count_neighbours_col(b, 1);
        count_neighbours_col(b, b->Nx - 1);
        count_neighbours_col(b, b->Nx - 2);
        update_state_col(b, 0);
        update_state_col(b, b->Nx - 1);

        // Start sending borders asynchroneously
        if (db->nodeRank & 1)
        {
            MPI_Isend(&(b->arr[0 * b->ld - 1]), b->ld, MPI_FLOAT,
                      leftNodeRank, SHIFT_LEFT_ODD, MPI_COMM_WORLD, &sendLeft);
            MPI_Irecv(&(b->arr[b->Nx * b->ld - 1]), b->ld, MPI_FLOAT,
                      rightNodeRank, SHIFT_LEFT_EVEN, MPI_COMM_WORLD, &recvRight);
            MPI_Isend(&(b->arr[(b->Nx - 1) * b->ld - 1]), b->ld, MPI_FLOAT,
                      rightNodeRank, SHIFT_RIGHT_ODD, MPI_COMM_WORLD, &sendRight);
            MPI_Irecv(&(b->arr[- 1 * b->ld - 1]), b->ld, MPI_FLOAT,
                      leftNodeRank, SHIFT_RIGHT_EVEN, MPI_COMM_WORLD, &recvLeft);

        }
        else
        {
            MPI_Irecv(&(b->arr[b->Nx * b->ld - 1]), b->ld, MPI_FLOAT,
                      rightNodeRank, SHIFT_LEFT_ODD, MPI_COMM_WORLD, &recvRight);
            MPI_Isend(&(b->arr[0 * b->ld - 1]), b->ld, MPI_FLOAT,
                      leftNodeRank, SHIFT_LEFT_EVEN, MPI_COMM_WORLD, &sendLeft);
            MPI_Irecv(&(b->arr[- 1 * b->ld - 1]), b->ld, MPI_FLOAT,
                      leftNodeRank, SHIFT_RIGHT_ODD, MPI_COMM_WORLD, &recvLeft);
            MPI_Isend(&(b->arr[(b->Nx - 1) * b->ld - 1]), b->ld, MPI_FLOAT,
                      rightNodeRank, SHIFT_RIGHT_EVEN, MPI_COMM_WORLD, &sendRight);
        }


        // Cover communication time by updating inner cells
        count_neighbours_block_col(b, 2, b->Nx - 2);

        for (int x = 1; x < b->Nx - 1; x++)
            update_state_col(b, x);


        if (db->nodeRank & 1)
        {
            MPI_Wait(&sendLeft, MPI_STATUS_IGNORE);
            MPI_Wait(&recvRight, MPI_STATUS_IGNORE);
            MPI_Wait(&sendRight, MPI_STATUS_IGNORE);
            MPI_Wait(&recvLeft, MPI_STATUS_IGNORE);
        }
        else
        {
            MPI_Wait(&recvRight, MPI_STATUS_IGNORE);
            MPI_Wait(&sendLeft, MPI_STATUS_IGNORE);
            MPI_Wait(&recvLeft, MPI_STATUS_IGNORE);
            MPI_Wait(&sendRight, MPI_STATUS_IGNORE);
        }


    }
}
