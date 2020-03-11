#ifndef MASTER_H
#define MASTER_H


void masterSimulationInit(int initN, int Ncell, int initInfection, int timeAll, int maxN);
// void masterTerminationCtrl(int maxN);

void startSquirrel(int initN, int initInfection, long seed);
void startLand(int initN, int initInfection);
void startTimer(int timeAll);
void startCTRL(int maxN);

#endif
