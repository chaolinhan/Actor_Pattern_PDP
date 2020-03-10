#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"
int inputCheck(int argc);
void readPara(int *initN, int *Ncell, int *maxN, int *initInfection,
              int *timeAll, char *argv[]);
