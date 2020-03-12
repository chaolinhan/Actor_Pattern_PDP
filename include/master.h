#ifndef MASTER_H
#define MASTER_H

// Start simulation
void masterSimulationInit(int initN, int Ncell, int initInfection, int timeAll, int maxN);

// Wait till end
void masterWait(void);

// Start squirrel actors of population initN and infection level initInfection
void startSquirrel(int initN, int initInfection, long seed);

// Start 16 land actors
void startLand(void);

// Start a timer to report time every month
void startTimer(void);

// Start a controller actor to count the population and determine termination
void startCTRL(void);

#endif
