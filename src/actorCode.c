#include "actorCode.h"


void actorCode(int initN, int Ncell, int maxN, int initInfection) {
	printf("actor started\n");
	int parentID = getCommandData();
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Status status;
	int role, flag = 0, actorStatus = 1;

	while (actorStatus) {
		MPI_Recv(&role, 1, MPI_INT, parentID, ROLE_TAG, MPI_COMM_WORLD, &status);
		printf("rank %d ROLE assigned: %d\n", rank, role);
		switch (role) {
			case ROLE_SQUIRREL: actorRUN(squirrelRUN, initN, Ncell, maxN, initInfection); break;
		}
		// MPI_Iprobe(parentID, ROLE_TAG, MPI_COMM_WORLD, &flag, &status);

		actorStatus = workerSleep();
	}
	// printf("test: sleep\n");
	return;
}
