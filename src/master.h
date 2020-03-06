#ifndef MASTER_H
#define MASTER_H

#include "actorCode.h"
#include "../lib/pool.h"
#include "../lib/ran2.h"
#include "../lib/squirrel-functions.h"

void MasterSimulationInit(int initN, int Ncell, int initInfection, int timeAll);
void startSquirrel(int initN, int initInfection, long seed);

#endif
