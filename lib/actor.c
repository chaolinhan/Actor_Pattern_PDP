#include "actor.h"

void actorRUN(void (*roleRUN)(int, int, int, int), int initN, int Ncell, int maxN, int initInfection) {
	roleRUN(initN, Ncell, maxN, initInfection);
}
