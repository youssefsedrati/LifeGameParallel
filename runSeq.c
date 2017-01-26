#include "runSeq.h"
#include "Board.h"

/* Public */

void run_seq(Board *b, int nbIterations)
{
    for (int iter = 0; iter < nbIterations; iter++)
    {
        // Count neighbours
        for (int x = 0; x < b->Nx; x++)
            count_neighbours_col(b, x);

        // Update cells
        for (int x = 0; x < b->Nx; x++)
            update_state_col(b, x);

        copy_state_col(b, -1, b->Nx - 1);
        copy_state_col(b, b->Nx, 0);
    }
}