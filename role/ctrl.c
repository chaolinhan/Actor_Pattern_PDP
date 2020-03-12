#include "../include/actorCode.h"
#include "../include/ctrl.h"
#include "../lib/actor.h"

#include "../lib/pool.h"
#include "../lib/ran2.h"
#include "../lib/squirrel-functions.h"
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void ctrlRUN(int maxN, int timeAll) {
	int rank, isAlive = 1;
	int flag, pop = 0;
	rank = actorGetID();
//  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	printf("CTRL on %d READY\n", rank);
	MPI_Status st;

	while (1) {
		// printf("CTRL working\n");
		// End time Control
//    MPI_Iprobe(TIMER_ID, TIMER_CTRL_TAG, MPI_COMM_WORLD, &flag, &st);
		if (actorProbe(TIMER_ID, TIMER_CTRL_TAG)) {
			isAlive = actorRecv(TIMER_ID, TIMER_CTRL_TAG).msg;
//      MPI_Recv(&isAlive, 1, MPI_INT, TIMER_ID, TIMER_CTRL_TAG, MPI_COMM_WORLD,
//               &st);
			sleep(1);
			printf("\tFINISHED: Simulation Stoped. Population: %d\n", pop);
			shutdownPool();
			return;
		}

		// Population control
//    MPI_Iprobe(MPI_ANY_SOURCE, POP_CTRL_TAG, MPI_COMM_WORLD, &flag, &st);
		if (actorProbe(ANY_SOURCE, POP_CTRL_TAG)) {
//      MPI_Recv(&isAlive, 1, MPI_INT, st.MPI_SOURCE, POP_CTRL_TAG,
//               MPI_COMM_WORLD, &st);
			isAlive = actorRecv(ANY_SOURCE, POP_CTRL_TAG).msg;
			if (isAlive == 1) {
				pop++;
				// printf("\tCTRL receive birth[V] signal from squirrel %d. pop: %d\n",
				//        st.MPI_SOURCE, pop);
			} else if (isAlive == 0) {
				pop--;
				// printf("\tCTRL receive death[X] signal from squirrel %d. pop: %d\n",
				//        st.MPI_SOURCE, pop);
			}

			if (pop >= maxN) {
				printf("\n\tERROR: Simulation Stoped (maximal allowed population "
							 "reached)\n");
				sleep(1);
				shutdownPool();
				break;
			}

		}

		if (actorStop()) {
			printf("CTRL should stop\n");
			break;
		}

	}
	printf("CTRL out loop\n");
}
