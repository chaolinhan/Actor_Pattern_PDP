#include "actor.h"


void actorCode(int *role) {
	int actorStatus = 1;
	printf("act start\n");
	int localRank, month=0;
	MPI_Comm_rank(MPI_COMM_WORLD, &localRank);
	printf("Actor ready %d\n", localRank);
	while (actorStatus) {

			printf("Do something\n");
			MPI_Recv(&month, 1, MPI_INT, getCommandData(), 33, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		// // MPI_Recv(&timerID, 1, MPI_INT, getCommandData(), 22, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		// // TODO: MAKE IT A FRAMEWORK
		// if (localRank == timerID) {
		// 	timerCode();
		// }
		// if (localRank != timerID) {
		//
		// }

		if (month == 4)
			actorStatus = workerSleep();
	}
	actorStatus = workerSleep();
	return;
}

void timerCode() {
	double tStart = MPI_Wtime(), tNow;
	int month = (int)(tNow-tStart)/5;
	int localRank;
	MPI_Comm_rank(MPI_COMM_WORLD, &localRank);
	while(MPI_Wtime()-tStart<=20.0) {
		tNow = MPI_Wtime();
		if ((int)(tNow-tStart)/5 != month) {
			month = (int)(tNow-tStart)/5;
			printf("Timer report: month %d\n", month);
			// MPI_Barrier(MPI_COMM_WORLD);
			// MPI_Bcast(&month, 1, MPI_INT, localRank, MPI_COMM_WORLD);
			printf("Broadcasted\n");
		}

	}
}
