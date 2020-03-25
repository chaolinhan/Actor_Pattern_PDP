#include <mpi.h>
#include <string.h>
#include "../lib/actor.h"
#include "../include/squirrel.h"
#include "../include/actorCode.h"
#include "../lib/squirrel-functions.h"

/**
 * Run squirrel actor's code
 * @param maxN maximum allowed number of squirrels
 * @param timeAll months to model
 */
void squirrelRUN(int maxN, int timeAll) {

	int isInfected, rank, isAlive = 1;
	int parentID = actorGetCreatorID();
	rank = actorGetID();

	actorSend(isAlive, CTRL_ID, POP_CTRL_TAG); // send born message to CTRL
	isInfected = actorRecv(parentID, INF_TAG).msg; // receive infection status

	// Receive initial position
	float bornPOS[2] = {0, 0};
	MPI_Status status;
	MPI_Recv(bornPOS, 2, MPI_FLOAT, parentID, POS_TAG, MPI_COMM_WORLD, &status);

	// Squirrel action begin
	int popInf[50], infLv[50];
	memset(popInf, 0, sizeof(popInf));
	memset(infLv, 0, sizeof(infLv));
	int cur = 0, infStep = 0;
	float x, y;
	long state = -1 - rank;
	x = bornPOS[0];
	y = bornPOS[1];
	int cellID;
	initialiseRNG(&state);

	// Squirrel steps
	while (isAlive) {
		// Die
		if (infStep >= 50) {
			if (willDie(&state)) {
				isAlive = 0;
				actorSend(isAlive, CTRL_ID, POP_CTRL_TAG);
				return;
			}
		}

		// Give birth every 50 steps
		if (cur == 0) {
			// Calculate average population
			int ii, totalPop = 0;
			float avg_pop;
			for (ii = 0; ii < 50; ii++)
				totalPop += popInf[ii];
			avg_pop = (float) totalPop / 50.;

			if (willGiveBirth(avg_pop, &state)) {
				// Create descendant
				int sID = actorCreate();
				actorSend(ROLE_SQUIRREL, sID, ROLE_TAG); // send role
				actorSend(0, sID, INF_TAG); // send infection status
				// Send born location
				float babyPOS[2];
				babyPOS[0] = x, babyPOS[1] = y;
				MPI_Bsend(babyPOS, 2, MPI_FLOAT, sID, POS_TAG, MPI_COMM_WORLD);
			}
		}

		// Get infected
		if (isInfected == 0) {
			int jj, totalInf = 0;
			float avgInf;
			// Calculate average infection level
			for (jj = 0; jj < 50; jj++)
				totalInf += infLv[jj];
			avgInf = (float) totalInf / 50.;

			if (willCatchDisease(avgInf, &state)) isInfected = 1;
		}

		// Step
		squirrelStep(x, y, &x, &y, &state);
		if (isInfected) infStep++;
		cellID = getCellFromPosition(x, y);

		// Update
		// Send step and infection message to Land
		actorSend(isInfected, getLandID(cellID), STEP_INF_TAG);
		popInf[cur] = actorRecv(getLandID(cellID), POP_INF_TAG).msg; // get population influx
		infLv[cur] = actorRecv(getLandID(cellID), INF_LV_TAG).msg; // get infection level
		// Move cursor
		cur++;
		if (cur >= 50) cur = cur % 50;

		if (actorStop()) break;
	}
}

/**
 * Get MPI rank from cell ID
 * @param cellID
 * @return MPI rank
 */
static int getLandID(int cellID) {
	return cellID + 2;
}