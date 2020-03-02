#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"
int inputCheck(int argc, char *argv[]);

void readPara(FILE *in, int *initN, int *Ncell, int *maxN, int *initInfection, int *timeAll);
