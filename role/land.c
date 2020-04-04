#include <stdio.h>
#include "../lib/actor.h"
#include "../include/actorCode.h"
#include "../include/land.h"

/**
 * Run Land actor's code
 * @param maxN maximum allowed number of squirrels
 * @param timeAll months to model
 */
void landRUN(int maxN, int timeAll) {
	int rank = actorGetID();

	// Land action begin
	int populationInflux[3] = {0, 0, 0},
			infectionLevel[3] = {0, 0, 0};                  // 3 months' count
	int moPopulationInflux = 0, moInfectionLevel = 0;   // monthly count
	int allPopulationInflux = 0, allInfectionLevel = 0; // total count

	int month = 0, cur = 0;
	int isInfected, tempMonth = 0;

	while (month <= timeAll - 1) {
		// Get month message if it comes
		if (actorProbe(TIMER_ID, MONTH_TAG)) {
			tempMonth = actorRecv(TIMER_ID, MONTH_TAG).msg;
		}

		// Month change
		if (tempMonth != month) {
			month = tempMonth;
			if (month >= 1) {
				populationInflux[cur] = moPopulationInflux;
				infectionLevel[cur] = moInfectionLevel;
				moPopulationInflux = 0;
				moInfectionLevel = 0;
				cur++;
				if (cur >= 3) cur -= 3;
				allPopulationInflux =
						populationInflux[0] + populationInflux[1] + populationInflux[2];
				allInfectionLevel =
						infectionLevel[0] + infectionLevel[1] + infectionLevel[2];
				printf("Land on rank %2d, month %2d \tinflux: %2d\tinfected: %2d,  %d  %d  %d  |  %d  %d  %d\n",
							 rank,
							 month,
							 allPopulationInflux,
							 allInfectionLevel,
							 populationInflux[0], populationInflux[1], populationInflux[2],
							 infectionLevel[0], infectionLevel[1], infectionLevel[2]
							 );
			}

		}

		// Communicate with Squirrels
		// Receive step and infection message for squirrels. a.k.a. handshake
		if (actorProbe(ANY_SOURCE, STEP_INF_TAG)) {
			struct actorMSG MSG = actorRecv(ANY_SOURCE, STEP_INF_TAG);
			isInfected = MSG.msg;
			if (isInfected == 1) {moInfectionLevel++;moPopulationInflux++;}
			if (isInfected == 0) {moPopulationInflux++;}
			// Send populationInflux and infectionLevel to squirrels
			actorSend(allPopulationInflux, MSG.src, POP_INF_TAG);
			actorSend(allInfectionLevel, MSG.src, INF_LV_TAG);
		}

		if (actorStop()) break;
	}
}
