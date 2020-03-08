#include "../lib/pool.h"
#include "../lib/squirrel-functions.h"
#include "actorCode.h"
#include "master.h"
#include "mpi.h"
#include "utility.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef ROLE_TAG
#define ROLE_TAG 22
#endif

#ifndef TIME_TAG
#define TIME_TAG 33
#endif

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);
  // Input
  FILE *in, *out;
  in = fopen(argv[1], "r+");
  int initN, Ncell, maxN, initInfection, timeAll;
  char s[20];
  int i = 0;

  // Read parameters from file.
  if (inputCheck(argc, argv) == 0) {
    printf("Check input\nUsage: model [input parameter files]\n");
  } else {
    char s[20];
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    fscanf(in, "%s%d\ncells:%d\nmax:%d\ninfected:%d\nmonth:%d", s, &initN,
           &Ncell, &maxN, &initInfection, &timeAll);
    if (rank == 0) {
      printf("Parameters read\n%d\t%d\t%d\t%d\t%d\n", initN, Ncell, maxN,
             initInfection, timeAll);
    }
  }

  // printf("initN = %d\n", initN);
  // Act
  // Actor
  int statusCode = processPoolInit();
  if (statusCode == 1) { // act as actor
    actorCode(initN, Ncell, maxN, initInfection, timeAll);
  }
  // Master
  if (statusCode == 2) { // act as master

    MasterSimulationInit(initN, Ncell, initInfection, timeAll);
    int pop = 0;
    int masterStatus = masterPoll();
    // Used as a block to seek for finalize comditions
    while (masterStatus) {

      int isAlive, flag;
      MPI_Status status;

      // Population control
      MPI_Iprobe(MPI_ANY_SOURCE, POP_CTRL_TAG, MPI_COMM_WORLD, &flag, &status);
      if (flag) {
        MPI_Recv(&isAlive, 1, MPI_INT, status.MPI_SOURCE, POP_CTRL_TAG,
                 MPI_COMM_WORLD, &status);
			  if (isAlive == 1) pop++;
				else if (isAlive == 0) pop--;
				else if (isAlive == -1) {
					sleep(1);
					printf("FINISHED: Simulation Stoped. Population: %d\n", pop);
					break;
				}

      }


			if (pop >= maxN) {
				printf("ERROR: Simulation Stoped (maximal allowed population reached)\n");
				break;
			}

      masterStatus = masterPoll();
    }
		printf("\t\tmaster break\n");

  }

  processPoolFinalise();
  MPI_Finalize();
  return 0;
}
