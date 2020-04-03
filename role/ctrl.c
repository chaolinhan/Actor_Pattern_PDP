#include <stdio.h>
#include "../include/actorCode.h"
#include "../include/ctrl.h"
#include "../lib/actor.h"

/**
 * Run CTRL (controller) actor's code
 * @param maxN maximum allowed number of squirrels
 * @param timeAll months to model
 */
void ctrlRUN(int maxN, int timeAll) {
	int rank, isAlive, pop = 0, month=0;
	rank = actorGetID();
	printf("CTRL on %d READY\n", rank);

	while (1) {
		// Monthly report and termination control
		if (actorProbe(TIMER_ID, ANY_TAG)) {
			month = actorRecv(TIMER_ID, ANY_TAG).msg;
			if (month == -1) {
				sleep(1);
				printf("\tFINISHED: Simulation Stoped. Population: %d\n", pop);
				actorAllStop();
				return;
			} else {
				printf("CTRL on rank %2d, month %2d \tpopulation: %d\n", rank, month, pop);
			}
		}

		// Population control
		// Receive squirrel birth or death message if there exists
		if (actorProbe(ANY_SOURCE, POP_CTRL_TAG)) {
			isAlive = actorRecv(ANY_SOURCE, POP_CTRL_TAG).msg;
			if (isAlive == 1) {
				pop++;
			} else if (isAlive == 0) {
				pop--;
			}

			if (pop >= maxN) {
				printf("\nERROR: Simulation Stoped\nMaximal allowed population "
							 "reached: %d\n", pop);
				sleep(1);
				printf("\nERROR: Simulation Stopped\nIf the simulation does not exit properly, use CTRL + C to terminate it.\n");
				actorAllStop();
				break;
			}

		}
		if (actorStop()) break;
	}
}
