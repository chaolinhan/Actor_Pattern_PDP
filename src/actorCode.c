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
//		int rank = actorGetID();
		// MPI_Iprobe(parentID, ROLE_TAG, MPI_COMM_WORLD, &flag, &st);
		// if (flag) {
		if (actorStop())
			break;
		// printf("\t%d recving R\n", rank);
		role = actorRecv(parentID, ROLE_TAG);
//    MPI_Recv(&role, 1, MPI_INT, parentID, ROLE_TAG, MPI_COMM_WORLD, &status);
		// printf("\t%d recved R\n", rank);
		// printf("rank %d ROLE assigned: %d\n", rank, role);
		switch (role) {
			case ROLE_SQUIRREL: actorRun(squirrelRUN, maxN, timeAll);
				break;
			case ROLE_LAND: actorRun(landRUN, maxN, timeAll);
				break;
			case ROLE_TIMER: actorRun(timerRUN, maxN, timeAll);
				break;
			case ROLE_CTRL: actorRun(ctrlRUN, maxN, timeAll);
				break;
			default: break;
		}
		// }

		// MPI_Iprobe(parentID, ROLE_TAG, MPI_COMM_WORLD, &flag, &status);
		// printf("\t\tactor %d going to sleep\n", rank);
		actorStatus = actorDie();
	}
	// printf("test: sleep\n");
}
