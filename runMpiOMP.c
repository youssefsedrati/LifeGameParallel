#include <omp.h>
#include <mpi.h>
#include "runMpiOMP.h"
#include "DistributedBoard.h"

void run_MPI_OMP_sync(DistributedBoard *db, int nbIterations, int nbThreads)
{
    omp_set_num_threads(nbThreads);
    int leftNodeRank = (db->nodeRank== 0) ? db->nbNodes - 1 : db->nodeRank - 1;
    int rightNodeRank = (db->nodeRank == db->nbNodes - 1) ? 0 : db->nodeRank + 1;
    Board *b = db->b;

#pragma omp parallel
    for (int iter = 0; iter < nbIterations; iter++)
    {
        // Count neighbours
#pragma omp for schedule(static)
        for (int x = 0; x < b->Nx; x++)
            count_neighbours_col(b, x);

        // Update cells
#pragma omp for schedule(static)
        for (int x = 0; x < b->Nx; x++)
            update_state_col(b, x);

#pragma omp single
        {
            MPI_Sendrecv(&(b->arr[(b->Nx - 1) * b->ld - 1]), b->ld, MPI_FLOAT, rightNodeRank, 42,
                         &(b->arr[- 1 * b->ld - 1]), b->ld, MPI_FLOAT, leftNodeRank, 42, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Sendrecv(&(b->arr[0 * b->ld - 1]), b->ld, MPI_FLOAT, leftNodeRank, 43,
                         &(b->arr[b->Nx * b->ld - 1]), b->ld, MPI_FLOAT, rightNodeRank, 43, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

#pragma omp barrier
    }
}

void run_MPI_OMP_async(DistributedBoard *db, int nbIterations, int nbThreads)
{

}