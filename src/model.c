#include "mpi.h"
#include "pool.h"
#include "squirrel-functions.h"
#include "utility.h"
#include "actor.h"
#include <stdio.h>
#include <stdlib.h>


#ifndef ROLE_TAG
#define ROLE_TAG 22
#endif

int main(int argc, char *argv[]) {
	MPI_Init(&argc, &argv);
// Input
  FILE *in, *out;
  in = fopen(argv[1], "r+");
  int initN, Ncell, maxN, initInfection, timeAll;
  char s[20];
  int i = 0;
	int timerID;
  if (inputCheck(argc, argv) == 0) {
    printf("Check input\nUsage: model [input parameter files]\n");
  } else {
    readPara(in, &initN, &Ncell, &maxN, &initInfection, &timeAll);
  }

// Act
// Actor
	int statusCode = processPoolInit();
	if (statusCode ==1) {// act as actor
		int role;
		actorCode(&role);
	}
// Master
  if (statusCode == 2) {// acto as master
		timerID = startWorkerProcess();
		MPI_Send(&timerID, 1, MPI_INT, timerID, ROLE_TAG, MPI_COMM_WORLD);
		//int p2 = startWorkerProcess();
	}

	processPoolFinalise();
	MPI_Finalize();
  return 0;
}
