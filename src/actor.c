#include "actor.h"


void actorCode(int role) {
	int actorStatus = 1;
	while (actorStatus) {
  	printf("act start\n");
		// TODO: MAKE IT A FRAMEWORK
		if (role == ROLE_TIMER) {
			timerCode();
		}
		actorStatus = workerSleep();
	}

	return;
}

void timerCode() {
	double tStart = MPI_Wtime(), tNow;
	int month = (int)(tNow-tStart)/5;
	while(MPI_Wtime()-tStart<=20.0) {
		tNow = MPI_Wtime();
		if ((int)(tNow-tStart)/5 != month) {
			month = (int)(tNow-tStart)/5;
			printf("Timer report: month %d\n", month);
		}

	}
}
