#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "../lib/actor.h"
#include "../include/squirrel.h"
#include "../lib/pool.h"

void squirrelRUN(int maxN, int timeAll) {
	// printf("️🐿️ acting\n");

	MPI_Status status;
	int isInfected, rank, flag, isAlive = 1;
	int parentID = actorGetCreatorID();
	rank = actorGetID();
//  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	// Let CTRL know
	actorSendMsg(isAlive, CTRL_ID, POP_CTRL_TAG);
//  MPI_Send(&isAlive, 1, MPI_INT, CTRL_ID, POP_CTRL_TAG, MPI_COMM_WORLD);

	// Receive infection status

	// printf("\t%d recving A\n", rank);
	isInfected = actorRecv(parentID, INF_TAG).msg;
//  MPI_Recv(&isInfected, 1, MPI_INT, parentID, INF_TAG, MPI_COMM_WORLD, &status);
// printf("\t%d recv A\n", rank);

	// Receive initial position
	float bornPOS[2] = {0, 0};
	if (actorStop()) return;
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
	int cellID = getCellFromPosition(x, y);
	initialiseRNG(&state);

	// Squirrel acts
	while (isAlive) {
		// printf("Squirrel %d is alive\n", rank);
		// Die
		if (infStep >= 50) {
			if (willDie(&state)) {
				// printf("\tSquirrel %d DEAD X\n", rank);
				isAlive = 0;
				actorSendMsg(isAlive, CTRL_ID, POP_CTRL_TAG);
//        MPI_Bsend(&isAlive, 1, MPI_INT, CTRL_ID, POP_CTRL_TAG, MPI_COMM_WORLD);
				return;
			}
		}

		// Give birth every 50 step
		if (cur == 0) {
			int ii, totalPop = 0;
			float avg_pop;
			for (ii = 0; ii < 50; ii++)
				totalPop += popInf[ii];
			avg_pop = (float) totalPop / 50.;

			if (willGiveBirth(avg_pop, &state)) {
				int sID = actorCreate();
				// printf("\tSquirrel %d giving birth to %d\n", rank, sID);
//        int msg = ROLE_SQUIRREL;
//        int babyIsInfected = 0;
				actorSendMsg(ROLE_SQUIRREL, sID, ROLE_TAG);
//        MPI_Bsend(&msg, 1, MPI_INT, sID, ROLE_TAG, MPI_COMM_WORLD);
				actorSendMsg(0, sID, INF_TAG);
//        MPI_Bsend(&babyIsInfected, 1, MPI_INT, sID, INF_TAG, MPI_COMM_WORLD);
				float babyPOS[2];
				babyPOS[0] = x, babyPOS[1] = y;
				MPI_Bsend(babyPOS, 2, MPI_FLOAT, sID, POS_TAG, MPI_COMM_WORLD);
			}
		}

		// Get infected
		if (isInfected == 0) {
			int jj, totalInf = 0;
			float avgInf;
			for (jj = 0; jj < 50; jj++)
				totalInf += infLv[jj];
			avgInf = (float) totalInf / 50.;

			if (willCatchDisease(avgInf, &state)) {
				// printf("\tSquirrel %d infected\n", rank);
				isInfected = 1;
			}
		}

		// Step
		float tempX, tempY;
		squirrelStep(x, y, &x, &y, &state);
		if (isInfected)
			infStep++;
		cellID = getCellFromPosition(x, y);

		// Update
//    int tempPopInf, tempInfLv;
		actorSendMsg(isInfected, cellID + 2, STEP_INF_TAG);
//    MPI_Bsend(&isInfected, 1, MPI_INT, cellID + 2, STEP_INF_TAG,
//              MPI_COMM_WORLD);
		// if (rank == 18) {
		// 	printf("\t now: (%.2f, %.2f), sending to %d\n", x, y, cellID+2);
		// }

		// printf("\t%d recving 1\n", rank);
//    MPI_Recv(&tempPopInf, 1, MPI_INT, cellID + 2, POP_INF_TAG, MPI_COMM_WORLD,
//             &status);
		// printf("\t%d recved 1\n", rank);
		popInf[cur] = actorRecv(cellID + 2, POP_INF_TAG).msg;

		// printf("\t%d recving 2\n", rank);
//    MPI_Recv(&tempInfLv, 1, MPI_INT, cellID + 2, INF_LV_TAG, MPI_COMM_WORLD,
//             &status);
		// printf("\t%d recved 2\n", rank);
		infLv[cur] = actorRecv(cellID + 2, INF_LV_TAG).msg;

		cur++;
		if (cur >= 50) cur = cur % 50;

		// should
		if (actorStop()) break;
	}
}
