#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include "../role/squirrel.h"
#ifndef ROLE_TIMER
#define ROLE_TIMER -1
#endif

#ifndef ROLE_SQUIRREL
#define ROLE_SQUIRREL 10
#endif

#ifndef ROLE_TAG
#define ROLE_TAG 10
#endif

#ifndef INF_TAG
#define INF_TAG 11
#endif

#ifndef POS_TAG
#define POS_TAG 12
#endif

void actorCode(int initN, int Ncell, int maxN, int initInfection);
