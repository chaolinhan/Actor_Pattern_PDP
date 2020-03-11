#ifndef MASTER_H
#define MASTER_H

#include <mpi.h>
#include <stdio.h>

#include "actorCode.h"
#include "../lib/pool.h"
#include "../lib/ran2.h"
#include "../lib/squirrel-functions.h"

void masterSimulationInit(int initN, int Ncell, int initInfection, int timeAll, int maxN);
// void masterTerminationCtrl(int maxN);

void startSquirrel(int initN, int initInfection, long seed);
void startLand(int initN, int initInfection);
void startTimer(int timeAll);
void startCTRL(int maxN);

#endif
