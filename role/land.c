#include "land.h"

void landRUN(int initN, int Ncell, int maxN, int initInfection, int timeAll) {
  MPI_Status status;
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  // printf("Land on %2d READY\n", rank);

  // Land action begin
  int populationInflux[3] = {0, 0, 0},
      infectionLevel[3] = {0, 0, 0};                // 3 months count
  int moPopulationInflux = 0, moInfectionLevel = 0; // monthly count
  int allPopulationInflux, allInfectionLevel;

  int month = 0, cur = 0;
  int flag, ii, isInfected, tempMonth = 0;

  while (month <= timeAll) {

    // Receive month from Timer
    MPI_Iprobe(TIMER_ID, MONTH_TAG, MPI_COMM_WORLD, &flag, &status);
    if (flag) {
      MPI_Recv(&tempMonth, 1, MPI_INT, TIMER_ID, MONTH_TAG, MPI_COMM_WORLD,
               &status);
    }

    // Month change
    if (tempMonth != month) {
      month = tempMonth;
      if (month >= 1) {
        populationInflux[cur] = moPopulationInflux;
        infectionLevel[cur] = moInfectionLevel;
        cur++;
        if (cur >= 3)
          cur -= 3;
        moPopulationInflux = 0;
        moInfectionLevel = 0;
        allPopulationInflux =
            populationInflux[0] + populationInflux[1] + populationInflux[2];
        allInfectionLevel =
            infectionLevel[0] + infectionLevel[1] + infectionLevel[2];
				printf("Land on rank %2d, month %2d \tinflux: %2d\tinfected: %2d\n", rank, month, allPopulationInflux, allInfectionLevel);
      }

    }

    // Communicate with Squirrels
    // TODO: 50
    // TODO: each iteration, one recv or all recv?
    // TODO: combine the handshake with the communicattion with Timer, or use a
    // actor fnction to achieve it
    MPI_Iprobe(MPI_ANY_SOURCE, STEP_INF_TAG, MPI_COMM_WORLD, &flag, &status);
    // Handshake success
    if (flag) {
			// printf("\tHand shaked, Land %d and Squirrel %d\n", rank, status.MPI_SOURCE);
      MPI_Recv(&isInfected, 1, MPI_INT, status.MPI_SOURCE, STEP_INF_TAG,
               MPI_COMM_WORLD, &status);
		  // printf("Received: %d, from: %d\n", isInfected, status.MPI_SOURCE);
      moPopulationInflux++;
			MPI_Bsend(&allPopulationInflux, 1, MPI_INT, status.MPI_SOURCE, POP_INF_TAG, MPI_COMM_WORLD);
			MPI_Bsend(&allInfectionLevel, 1, MPI_INT, status.MPI_SOURCE, INF_LV_TAG, MPI_COMM_WORLD);
      if (isInfected == 1)
        moInfectionLevel++;
    }

	  if(shouldWorkerStop()) break;
  }

  return;
}
