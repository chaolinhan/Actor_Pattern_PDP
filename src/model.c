#include "mpi.h"
#include "pool.h"
#include "squirrel-functions.h"
#include "utility.h"
#include "actor.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	MPI_Init(&argc, &argv);
// Input
  FILE *in, *out;
  in = fopen(argv[1], "r+");
  int initN, Ncell, maxN, initInfection, timeAll;
  char s[20];
  int i = 0;
  if (inputCheck(argc, argv) == 0) {
    printf("Check input\nUsage: model [input parameter files]\n");
  } else {
    readPara(in, &initN, &Ncell, &maxN, &initInfection, &timeAll);
  }
	int statusCode = processPoolInit();
	if (statusCode ==1) {// act as actor
		actorCode(-1);
	}
  if (statusCode == 2) {// acto as master
		int p1 = startWorkerProcess();
		//int p2 = startWorkerProcess();
	}

	processPoolFinalise();
	MPI_Finalize();
  return 0;
}
