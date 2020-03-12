#include <mpi.h>
#include <stdio.h>
#include "../lib/actor.h"
#include "../include/timer.h"
#include "../include/actorCode.h"
#include "../lib/pool.h"

void timerRUN(int maxN, int timeAll) {
	int rank, month, ii;
	double tStart, tNow;
	rank = actorGetID();
//  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	printf("Timer on %d READY, simulation length: %d\n", rank, timeAll);
	month = 0;
	tStart = MPI_Wtime();

	// Timer acts
	while (month <= timeAll - 1) {
		// printf("T\n");
		if (actorStop()) return;
		tNow = MPI_Wtime();
		// Month change every two seconds
		if ((int) (tNow - tStart) >= month + 1) {
			month++;
			printf("\n\t\t    Timer report: month %2d    \n", month);

			// Send month to Land
			for (ii = 2; ii <= 17; ii++) {
				actorSendMsg(month, ii, MONTH_TAG);
//				MPI_Bsend(&month, 1, MPI_INT, ii, MONTH_TAG, MPI_COMM_WORLD);
			}
		}

		if (actorStop()) return;

	}
	// Send end signal to MASTER
	if (month > timeAll - 1) {
		int isAlive = -1;
		// printf("Timer sending end signal\n");
		actorSendMsg(isAlive, CTRL_ID, TIMER_CTRL_TAG);
//		MPI_Bsend(&isAlive, 1, MPI_INT, CTRL_ID, TIMER_CTRL_TAG, MPI_COMM_WORLD);
		// shutdownPool();
	}

}
