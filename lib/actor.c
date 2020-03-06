#include "actor.h"

void actorRUN(void (*roleRUN)(int, int, int, int, int), int initN, int Ncell, int maxN, int initInfection, int timeAll) {
	roleRUN(initN, Ncell, maxN, initInfection, timeAll);
}
