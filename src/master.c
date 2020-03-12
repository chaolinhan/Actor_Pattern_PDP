#include "../include/master.h"
#include <mpi.h>
#include <stdio.h>

#include "../include/actorCode.h"
#include "../lib/pool.h"
#include "../lib/actor.h"
#include "../lib/squirrel-functions.h"

/**
 * Start simulation
 * @param initN initial number of squirrels
 * @param Ncell Number of cells
 * @param initInfection initial number of infected squirrels
 * @param timeAll months to model
 * @param maxN maximum allowed number of squirrels
 */
void masterSimulationInit(int initN, int Ncell, int initInfection,
													int timeAll, int maxN) {
	long seed = 20;
	printf("Simulation started\n");
	initialiseRNG(&seed);
	startTimer();   // MPI rank: 1
	startLand();    // MPI rank: 2 — 17
	startCTRL();    // MPI rank: 18
	startSquirrel(initN, initInfection, seed); // MPI rank: 19 and onwards
}

/**
 * Start squirrel actors of population initN and infection level initInfection
 * @param initN initial number of squirrels
 * @param initInfection initial number of infected squirrels
 * @param seed random seed
 */
void startSquirrel(int initN, int initInfection, long seed) {
	int i, sID, msg, isInfected;
	int Ninf = initInfection;
	printf("Initialise %d 🐿️\n", initN);

	// Create squirrel actors
	for (i = 0; i < initN; i++) {
		sID = actorCreate();
		actorSend(ROLE_SQUIRREL, sID, ROLE_TAG); // send role message
		if (Ninf > 0) {
			isInfected = 1;
			Ninf--;
		} else
			isInfected = 0;
		actorSend(isInfected, sID, INF_TAG);  // send infection status

		// Initialise and send born location
		float bornPOS[2] = {0, 0};
		squirrelStep(0, 0, &bornPOS[0], &bornPOS[1], &seed);
		MPI_Bsend(bornPOS, 2, MPI_FLOAT, sID, POS_TAG, MPI_COMM_WORLD);
	}
}

/**
 * Start 16 land actors
 */
void startLand(void) {
	int lID, i;
	// TODO : Ncell
	// Create land actors
	for (i = 0; i < 16; i++) {
		lID = actorCreate();
		actorSend(ROLE_LAND, lID, ROLE_TAG); // send role message
	}
}

/**
 * Start a timer to report time every month
 */
void startTimer(void) {
	int tID;
	// Create timer actor
	tID = actorCreate();
	actorSend(ROLE_TIMER, tID, ROLE_TAG); // send role message
}

/**
 * Start a controller actor to count the population and determine termination
 */
void startCTRL(void) {
	int CID;
	// Create CTRL actor
	CID = actorCreate();
	actorSend(ROLE_CTRL, CID, ROLE_TAG); // send role message
}

/**
 * Wait till end
 */
void masterWait(void) {
	int masterStatus = 1;
	while (masterStatus) {
		masterStatus = masterPoll();
	}
}
