#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/timer.h"
#include "../include/actorCode.h"
#include "../lib/pool.h"
#include "../lib/ran2.h"
#include "../lib/squirrel-functions.h"

void timerRUN(int initN, int Ncell, int maxN, int initInfection, int timeAll) {
  MPI_Status status;
  int rank, month, ii;
  double tStart, tNow;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  printf("Timer on %d READY, simulation length: %d\n", rank, timeAll);
  month = 0;
  tStart = MPI_Wtime();

	// Timer acts
  while (month <= timeAll-1) {
		// printf("T\n");
    tNow = MPI_Wtime();
		// Month change every two seconds
    if ((int)(tNow - tStart) >= month+1) {
      month++;
      printf("\n\t\t    Timer report: month %2d    \n", month);

			// Send month to Land
			for (ii = 2; ii<=17; ii++) {
				MPI_Bsend(&month, 1, MPI_INT, ii, MONTH_TAG, MPI_COMM_WORLD);
			}
    }

		if(shouldWorkerStop()) return;

  }
  // Send end signal to MASTER
	int isAlive = -1;
	// printf("Timer sending end signal\n");
	MPI_Bsend(&isAlive, 1, MPI_INT, MASTER_ID, TIMER_CTRL_TAG, MPI_COMM_WORLD);
	shutdownPool();
  return;
}
