#include "squirrel.h"

void squirrelRUN(int initN, int Ncell, int maxN, int initInfection, int timeAll) {
	// printf("️🐿️ acting\n");
	MPI_Status status;
	int isInfected, rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	// Receive infection status
	MPI_Recv(&isInfected, 1, MPI_INT, MPI_ANY_SOURCE, INF_TAG, MPI_COMM_WORLD, &status);
	// printf("isInfected: %d\n", isInfected);

	// Receive initial position
	float bornPOS[2] = {0, 0};
	MPI_Recv(bornPOS, 2, MPI_FLOAT, MPI_ANY_SOURCE, POS_TAG, MPI_COMM_WORLD, &status);
	// printf("(%.2f, %.2f)\n", bornPOS[0], bornPOS[1]);
	printf("🐿️ %2d READY\tisInfected: %d\tposition: (%.2f, %.2f)\n", rank, isInfected, bornPOS[0], bornPOS[1]);
}
