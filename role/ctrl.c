#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/ctrl.h"
#include "../lib/pool.h"
#include "../lib/ran2.h"
#include "../lib/squirrel-functions.h"

void ctrlRUN (void) {
	int Alive = 1;
	int pop = 0;
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	printf("CTRL on %d READY\n", rank);

	// while (Alive) {
	//
	// 	if ( shouldWorkerStop()) break;
	// }
  return;
}
