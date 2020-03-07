#include "land.h"

void landRUN(int initN, int Ncell, int maxN, int initInfection, int timeAll) {
	MPI_Status status;
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	printf("Land on rank %d READY\n", rank);

	// Land action begin
}
