#include <mpi.h>
#include <stdio.h>

#include "../lib/actor.h"
#include "../include/timer.h"
#include "../include/actorCode.h"

/**
 * Run timer actor's code
 * @param maxN maximum allowed number of squirrels
 * @param timeAll months to model
 */
void timerRUN(int maxN, int timeAll) {
	int rank, month = 0, ii;
	double tStart, tNow;
	rank = actorGetID();
	printf("Timer on %d READY, simulation length: %d\n", rank, timeAll);

	// Timer acts
	tStart = MPI_Wtime();
	while (month <= timeAll - 1) {
		if (actorStop()) return;
		tNow = MPI_Wtime();

		// Month changes every second
		if ((int) (tNow - tStart) >= month + 1) {
			month++;
			printf("\n\t\t    Timer report: month %2d    \n", month);

			// Send month to Land
			for (ii = 2; ii <= 17; ii++) actorSend(month, ii, MONTH_TAG);
		}
		if (actorStop()) return;
	}

	// Send end signal to CTRL
	if (month > timeAll - 1) {
		int isAlive = -1;
		actorSend(isAlive, CTRL_ID, TIMER_CTRL_TAG);
	}
}

