#include "squirrel.h"

void squirrelRUN(int initN, int Ncell, int maxN, int initInfection,
                 int timeAll) {
  // printf("️🐿️ acting\n");
  MPI_Status status;
  int isInfected, rank, isAlive = 1;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // Receive infection status
  MPI_Recv(&isInfected, 1, MPI_INT, MPI_ANY_SOURCE, INF_TAG, MPI_COMM_WORLD,
           &status);

  // Receive initial position
  float bornPOS[2] = {0, 0};
  MPI_Recv(bornPOS, 2, MPI_FLOAT, MPI_ANY_SOURCE, POS_TAG, MPI_COMM_WORLD,
           &status);
  printf("🐿️ on %2d READY\tisInfected: %d\tposition: (%.2f, %.2f)\n", rank,
         isInfected, bornPOS[0], bornPOS[1]);

  // // Receive land actor rank
	// int landID;
	// MPI_Recv(&landID, 1, MPI_INT, MPI_ANY_SOURCE, LAND_TAG, MPI_COMM_WORLD,
	// 				 &status);

  // Squirrel action begin
  int popInf[50], infLv[50];
  memset(popInf, 0, sizeof(popInf));
  memset(infLv, 0, sizeof(infLv));
  int cur = 0, infStep = 0, x, y;
	long state = -1 - rank;
  x = bornPOS[0];
  y = bornPOS[1];
	int cellID = getCellFromPosition(x, y);
  initialiseRNG(&state);

  //Squirrel acts
  while (isAlive) {

    // Die
    if (infStep >= 50) {
      if (willDie(&state)) {
        return;
      }
    }

    // Give birth every 50 step
		if (cur == 0) {
			int ii, totalPop = 0;
			float avg_pop;
			for (ii = 0; ii < 50; ii++)
				totalPop += popInf[ii];
			avg_pop = (float)totalPop / 50.;

			if (willGiveBirth(avg_pop, &state)) {
				int sID = startWorkerProcess();
				int msg = ROLE_SQUIRREL;
				int isInfected = 0;
				MPI_Bsend(&msg, 1, MPI_INT, sID, ROLE_TAG, MPI_COMM_WORLD);
				MPI_Bsend(&isInfected, 1, MPI_INT, sID, INF_TAG, MPI_COMM_WORLD);
				float babyPOS[2];
				babyPOS[0] = x, babyPOS[1] = y;
				MPI_Bsend(babyPOS, 2, MPI_FLOAT, sID, POS_TAG, MPI_COMM_WORLD);
			}
		}


    // Get infected
    int jj, totalInf = 0;
    float avgInf;
    for (jj = 0; jj < 50; jj++)
      totalInf += infLv[jj];
    avgInf = (float)totalInf / 50.;

		if (!isInfected && willCatchDisease(avgInf, &state)) {
			isInfected = 1;
		}

		// Step
		float tempX, tempY;
		squirrelStep(x, y, &tempX, &tempY, &state);
		x = tempX;
		y = tempY;
		cellID = getCellFromPosition(x, y);

		// Update
		int tempPopInf, tempInfLv;
		MPI_Bsend(isInfected, 0, MPI_INT, cellID+2, STEP_INF_TAG, MPI_COMM_WORLD);
		MPI_Recv(tempPopInf, 1, MPI_INT, cellID+2, POP_INF_TAG, MPI_COMM_WORLD, &status);
		MPI_Recv(tempInfLv, 1, MPI_INT, cellID+2, INF_LV_TAG, MPI_COMM_WORLD, &status);
		popInf[cur] = tempPopInf;
		infLv[cur] = tempInfLv;
		cur++;
		if ( cur >=50 ) cur = cur % 50;

		// should
		isAlive = shouldWorkerStop();
  }




}
