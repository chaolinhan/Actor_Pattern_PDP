#ifndef UTILITY_H
#define UTILITY_H

// Check if the input parameter number is legal
int inputCheck(int argc);

// Read parameters
void readPara(int *initN, int *Ncell, int *maxN, int *initInfection,
							int *timeAll, char *argv[]);

#endif