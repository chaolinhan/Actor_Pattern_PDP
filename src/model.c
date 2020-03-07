#include "../lib/pool.h"
#include "../lib/squirrel-functions.h"
#include "actorCode.h"
#include "mpi.h"
#include "master.h"
#include "utility.h"
#include <stdio.h>
#include <stdlib.h>

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
		fscanf(in, "%s%d\ncells:%d\nmax:%d\ninfected:%d\nmonth:%d", s, &initN, &Ncell, &maxN, &initInfection, &timeAll);
		if (rank == 0) {
			printf("Parameters read\n%d\t%d\t%d\t%d\t%d\n", initN, Ncell, maxN, initInfection, timeAll);
		}
  }

	//printf("initN = %d\n", initN);
  // Act
  // Actor
  int statusCode = processPoolInit();
  if (statusCode == 1) { // act as actor
    actorCode(initN, Ncell, maxN, initInfection, timeAll);
  }
  // Master
  if (statusCode == 2) { // act as master

		MasterSimulationInit(initN, Ncell, initInfection, timeAll);
    int masterStatus = masterPoll();

		// Used as a block to seek for finalize comditions
    while (masterStatus) {
      masterStatus = masterPoll();
    }
  }

  processPoolFinalise();
  MPI_Finalize();
  return 0;
}
