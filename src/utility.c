#include <stdio.h>
#include "mpi.h"
#include "../include/utility.h"

/**
 * Check if the input parameter number is legal
 * @param argc
 * @return 1: legal; 0: illegal
 */
int inputCheck(int argc) {
	if (argc == 2)
		return 1;
	else
		return 0;
}

/**
 * Read parameters
 */
void readPara(int *initN, int *Ncell, int *maxN, int *initInfection,
							int *timeAll, char *argv[]) {
	FILE *in;
	in = fopen(argv[1], "r+");
	char s[20];
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	fscanf(in, "%s%d\ncells:%d\nmax:%d\ninfected:%d\nmonth:%d", s, initN, Ncell,
				 maxN, initInfection, timeAll);

	// Check parameter values' range
	if (*initN < 0 || *Ncell < 0 || *maxN < *initN || *initInfection > *initN || *timeAll <= 1) {
		if (rank == 0) {
			printf("\nERROR: check parameter values\nNow running on default parameters\n\n");
		}
		*initN = 34;
		*initInfection = 2;
		*Ncell = 16;
		*timeAll = 10;
		*maxN = 200;
		sleep(2);
	}
	if (rank == 0) {
		printf("Parameters read\n%d\t%d\t%d\t%d\t%d\n", *initN, *Ncell, *maxN,
					 *initInfection, *timeAll);
	}
}
