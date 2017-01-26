cmake_minimum_required(VERSION 3.6)
project(funWithLife)

set(SOURCE_FILES main.c Board.h Board.c runSeq.h runSeq.c runPthreads.h runPthreads.c runMpiSeq.c runMpiSeq.h DistributedBoard.c DistributedBoard.h runOMP.c runOMP.h runMpiOMP.c runMpiOMP.h)
add_executable(funWithLife ${SOURCE_FILES})

set(OMP_CFLAGS "-fopenmp")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OMP_CFLAGS} -std=c11 -O3")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}")
find_package(MKL REQUIRED)
message(STATUS MKL found: ${MKL_ROOT_DIR})
include_directories(${MKL_INCLUDE_DIR})
target_link_libraries(funWithLife ${MKL_LIBRARIES})

find_package(MPI REQUIRED)
message(STATUS MPI found: ${MPI_C_COMPILER})
include_directories(${MPI_C_INCLUDE_PATH})
target_link_libraries(funWithLife ${MPI_C_LIBRARIES})


target_link_libraries(funWithLife m)

