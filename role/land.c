#include <mpi.h>
#include <stdio.h>
#include "../lib/actor.h"
#include "../include/actorCode.h"
#include "../include/land.h"

void landRUN(int maxN, int timeAll) {
	MPI_Status status;
	int rank = actorGetID();
//  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	// printf("Land on %2d READY\n", rank);

	// Land action begin
	int populationInflux[3] = {0, 0, 0},
			infectionLevel[3] = {0, 0, 0};                // 3 months count
	int moPopulationInflux = 0, moInfectionLevel = 0; // monthly count
	int allPopulationInflux = 0, allInfectionLevel = 0;

	int month = 0, cur = 0;
	int isInfected, tempMonth = 0;

	while (month <= timeAll - 1) {
		// printf("Land %d is alive\n", rank);
		// Receive month from Timer
//    MPI_Iprobe(TIMER_ID, MONTH_TAG, MPI_COMM_WORLD, &flag, &status);
		if (actorProbe(TIMER_ID, MONTH_TAG)) {
			if (actorStop()) return;
			tempMonth = actorRecv(TIMER_ID, MONTH_TAG).msg;
//      MPI_Recv(&tempMonth, 1, MPI_INT, TIMER_ID, MONTH_TAG, MPI_COMM_WORLD,
//               &status);
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
				printf("Land on rank %2d, month %2d \tinflux: %2d\tinfected: %2d\n",
							 rank,
							 month,
							 allPopulationInflux,
							 allInfectionLevel);
			}

		}

		// Communicate with Squirrels
		// TODO: 50
		// TODO: each iteration, one recv or all recv?
		// TODO: combine the handshake with the communicattion with Timer, or use a
		// actor fnction to achieve it
//    MPI_Iprobe(MPI_ANY_SOURCE, STEP_INF_TAG, MPI_COMM_WORLD, &flag, &status);
		// Handshake success
		if (actorProbe(ANY_SOURCE, STEP_INF_TAG)) {
			// printf("\tHand shaked, Land %d and Squirrel %d\n", rank, status.MPI_SOURCE);
			if (actorStop()) break;
			struct actorMSG MSG = actorRecv(MPI_ANY_SOURCE, STEP_INF_TAG);
			isInfected = MSG.msg;
			//      MPI_Recv(&isInfected, 1, MPI_INT, status.MPI_SOURCE, STEP_INF_TAG,
//               MPI_COMM_WORLD, &status);
			// printf("Received: %d, from: %d\n", isInfected, status.MPI_SOURCE);
			moPopulationInflux++;
			actorSendMsg(allPopulationInflux, MSG.src, POP_INF_TAG);
//			MPI_Bsend(&allPopulationInflux, 1, MPI_INT, status.MPI_SOURCE, POP_INF_TAG, MPI_COMM_WORLD);
			actorSendMsg(allInfectionLevel, MSG.src, INF_LV_TAG);
//			MPI_Bsend(&allInfectionLevel, 1, MPI_INT, status.MPI_SOURCE, INF_LV_TAG, MPI_COMM_WORLD);
			if (isInfected == 1)
				moInfectionLevel++;
		}

		if (actorStop()) break;
	}
	// printf("Land %2d out loop\n", rank);
}
