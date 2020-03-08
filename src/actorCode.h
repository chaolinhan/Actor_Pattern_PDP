#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include "../role/squirrel.h"
#include "../role/land.h"
#include "../role/timer.h"

// RANK

#ifndef MASTER_ID
#define MASTER_ID 0
#endif

#ifndef TIMER_ID
#define TIMER_ID 1
#endif

// MASTER to ACTOR ROLE code
#ifndef ROLE_TIMER
#define ROLE_TIMER 30
#endif

#ifndef ROLE_SQUIRREL
#define ROLE_SQUIRREL 10
#endif

#ifndef ROLE_LAND
#define ROLE_LAND 20
#endif

// MASTER to Squirrel TAG
#ifndef ROLE_TAG
#define ROLE_TAG 10
#endif

#ifndef INF_TAG
#define INF_TAG 11
#endif

#ifndef POS_TAG
#define POS_TAG 12
#endif

#ifndef LAND_TAG
#define LAND_TAG 13
#endif

#ifndef POP_CTRL_TAG
#define POP_CTRL_TAG 14
#endif

// Squirrel to Land TAG
#ifndef STEP_INF_TAG
#define STEP_INF_TAG 20
#endif

#ifndef POP_INF_TAG
#define POP_INF_TAG 21
#endif

#ifndef INF_LV_TAG
#define INF_LV_TAG 22
#endif

// Land to Timer TAG
#ifndef MONTH_TAG
#define MONTH_TAG 30
#endif

void actorCode(int initN, int Ncell, int maxN, int initInfection, int timeAll);
