#include "actor.h"


void actorCode(int *role) {
	int actorStatus = 1;
	while (actorStatus) {
  	printf("act start\n");
		int timerID, localRank;
		MPI_Comm_rank(MPI_COMM_WORLD, &localRank);


		MPI_Recv(&timerID, 1, MPI_INT, getCommandData(), 22, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		// TODO: MAKE IT A FRAMEWORK
		if (localRank == timerID) {
			timerCode();
		}
		if (localRank != timerID) {
			int month = 0, oldMonth = 0;
			while (oldMonth < 3) {
				if (month !=oldMonth) {
					oldMonth = month;
					printf("Month %d signal received\n", oldMonth);
				}
			}
		}

		actorStatus = workerSleep();
	}

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
