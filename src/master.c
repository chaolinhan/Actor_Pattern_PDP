#include "master.h"

void MasterSimulationInit(int initN, int Ncell, int initInfection, int timeAll) {
	long seed = 20;
	printf("Simulation started\ninitN = %d\n", initN);
	initialiseRNG(&seed);
	// startTimer(timeAll);
	startSquirrel(initN, initInfection, seed);
	// startLand();
}

void startSquirrel(int initN, int initInfection, long seed) {
	int i, sID, msg, isInfected;
	int Ninf = initInfection;
	printf("Initialise %d 🐿️\n", initN);
	for (i=0; i<initN; i++) {

		sID = startWorkerProcess();
		// actorSEND(ROLE_SQUIRREL, sID, ROLE_TAG);
		// Send role
		msg = ROLE_SQUIRREL;
		MPI_Bsend(&msg, 1, MPI_INT, sID, ROLE_TAG, MPI_COMM_WORLD);

		// Send infection status
		// if (Ninf>0) {
		// 	isInfected = 1;
		// 	Ninf--;
		// } else
		// 	isInfected = 0;
		// actorSEND(isInfected, sID, INF_TAG);
		//
		// // Initialise and send born location
		// float bornPOS[2] = {0, 0};
		// float diff=ran2(seed);
    // bornPOS[0] = (bornPOS[0]+diff)-(int)(bornPOS[0]+diff);
		// diff=ran2(seed);
		// bornPOS[1] = (bornPOS[1]+diff)-(int)(bornPOS[1]+diff);
		// MPI_Bsend(bornPOS, 2, MPI_FLOAT, sID, POS_TAG, MPI_COMM_WORLD);

	}
}
