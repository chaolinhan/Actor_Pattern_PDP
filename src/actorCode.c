#include "../lib/actor.h"
#include "../include/actorCode.h"
#include "../include/land.h"
#include "../include/squirrel.h"
#include "../include/timer.h"
#include "../include/ctrl.h"

/**
 * Run actor code according to roles
 * @param initN initial number of squirrels
 * @param Ncell Number of cells
 * @param initInfection initial number of infected squirrels
 * @param maxN maximum allowed number of squirrels
 * @param timeAll months to model
 */
void actorCode(int initN, int Ncell, int maxN, int initInfection, int timeAll) {
	int actorStatus = 1;

	while (actorStatus) {
		// Receive role message
		int role;
		int parentID = actorGetCreatorID();
		role = actorRecv(parentID, ROLE_TAG).msg;

		// Assign tasks according to role
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

		// Actor dies after task finished
		actorStatus = actorDie();
	}
}
