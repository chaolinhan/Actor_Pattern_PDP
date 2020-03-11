#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#include "../lib/actor.h"
#include "../include/actorCode.h"
#include "../include/land.h"
#include "../include/squirrel.h"
#include "../include/timer.h"
#include "../include/ctrl.h"

void actorCode(int initN, int Ncell, int maxN, int initInfection, int timeAll) {
  // printf("\t\t\tactor started\n");
  int actorStatus = 1;

  while (actorStatus) {
    // printf("\tactor awaken\n");
    int role, flag;
    int parentID = actorGetCreatorID();
    MPI_Status st;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;
    // MPI_Iprobe(parentID, ROLE_TAG, MPI_COMM_WORLD, &flag, &st);
    // if (flag) {
    if (shouldWorkerStop())
      break;
			// printf("\t%d recving R\n", rank);
    MPI_Recv(&role, 1, MPI_INT, parentID, ROLE_TAG, MPI_COMM_WORLD, &status);
		// printf("\t%d recved R\n", rank);
    // printf("rank %d ROLE assigned: %d\n", rank, role);
    switch (role) {
    case ROLE_SQUIRREL:
      // actorRUN(squirrelRUN, initN, Ncell, maxN, initInfection, timeAll);
      squirrelRUN(initN, Ncell, maxN, initInfection, timeAll);
      break;
    case ROLE_LAND:
      actorRUN(landRUN, initN, Ncell, maxN, initInfection, timeAll);
      break;
    case ROLE_TIMER:
      actorRUN(timerRUN, initN, Ncell, maxN, initInfection, timeAll);
      break;
    case ROLE_CTRL:
      // actorRUN(ctrlRUN, initN, Ncell, maxN, initInfection, timeAll);
			ctrlRUN(maxN);
			break;
    }
    // }

    // MPI_Iprobe(parentID, ROLE_TAG, MPI_COMM_WORLD, &flag, &status);
    // printf("\t\tactor %d going to sleep\n", rank);
    actorStatus = workerSleep();
  }
  // printf("test: sleep\n");
  return;
}
