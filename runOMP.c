#include "runOMP.h"
#include "Board.h"

void run_OMP(Board *b, int nbIterations, int nbThreads)
{
    omp_set_num_threads(nbThreads);

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
        copy_state_col(b, -1, b->Nx - 1);
        #pragma omp single
        copy_state_col(b, b->Nx, 0);

        #pragma omp barrier
    }
}