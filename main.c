#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

#include "Board.h"
#include "runSeq.h"
#include "runPthreads.h"
#include "runMpiSeq.h"
#include "DistributedBoard.h"
#include "runOMP.h"
#include "runMpiOMP.h"

static void printUsage(char** argv)
{
    printf("Usage: %s {SEQ|OMP|PTHREADS|MPI_SEQ_ASYNC|MPI_SEQ_SYNC} nb_iterations board_size nb_threads\n"
                   "With board_size must be a multiple of nb_threads * nb_mpi_nodes\n", argv[0]);
}

int main(int argc, char** argv)
{
    MPI_Init(NULL, NULL);

    if (argc != 5)
    {
        printUsage(argv);
        return EXIT_FAILURE;
    }

    int nbIters = atoi(argv[2]);
    int N = atoi(argv[3]);
    int nbThreads = atoi(argv[4]);
    int nbNodes;
    MPI_Comm_size(MPI_COMM_WORLD, &nbNodes);

    if (N % (nbNodes * nbThreads) != 0)
    {
        printUsage(argv);
        return EXIT_FAILURE;
    }

    // The initial state of the board is a cross
    DistributedBoard db;
    init_distributed_board(&db, N);
    set_distributed_row(&db, N/2, 1);
    set_distributed_col(&db, N/2, 1);

    struct timeval start, stop;
    gettimeofday(&start, NULL);

    if      (strcmp(argv[1], "SEQ") == 0)           run_seq(db.b, nbIters);
    else if (strcmp(argv[1], "OMP") == 0)           run_OMP(db.b, nbIters, nbThreads);
    else if (strcmp(argv[1], "PTHREADS") == 0)      run_pthreads(db.b, nbIters, nbThreads);
    else if (strcmp(argv[1], "MPI_SEQ_SYNC") == 0)  run_MPI_seq_sync(&db, nbIters);
    else if (strcmp(argv[1], "MPI_SEQ_ASYNC") == 0) run_MPI_seq_async(&db, nbIters);
    else if (strcmp(argv[1], "MPI_OMP_SYNC") == 0)  run_MPI_OMP_sync(&db, nbIters, nbThreads);
    else if (strcmp(argv[1], "MPI_OMP_ASYNC") == 0) run_MPI_OMP_async(&db, nbIters, nbThreads);
    else {
        printf("Unknown version\n");
        printf("Usage: %s {REF|OMP|PTHREADS|MPI_SEQ_SYNC|MPI_SEQ_ASYNC_MPI_OMP_SYNC|MPI_SEQ_SYNC} nb_iterations board_size nb_threads\n", argv[0]);
    }

    gettimeofday(&stop, NULL);
    if (db.nodeRank == 0) printf("\n%e s\n\n", stop.tv_sec - start.tv_sec + 0.000001 * (stop.tv_usec - start.tv_usec));

//    for (int i = 0; i < db.nbNodes; i++) {
//        if (db.nodeRank == i) {
//            output_board(stdout, db.b);
//            printf("\n");
//        }
//        MPI_Barrier(MPI_COMM_WORLD);
//    }



    free_distributed_board(&db);

    MPI_Finalize();
    return 0;
}