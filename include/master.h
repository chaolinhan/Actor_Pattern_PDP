#ifndef MASTER_H
#define MASTER_H

#include "actorCode.h"
#include "../lib/pool.h"
#include "../lib/ran2.h"
#include "../lib/squirrel-functions.h"

void masterSimulationInit(int initN, int Ncell, int initInfection, int timeAll);
void masterTerminationCtrl(int maxN);

void startSquirrel(int initN, int initInfection, long seed);
int startLand(int initN, int initInfection);
void startTimer(int timeAll);

#endif
