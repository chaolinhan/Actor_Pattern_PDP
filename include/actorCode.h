#ifndef ACTORCODE_H
#define ACTORCODE_H

#include <stdlib.h>

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

#ifndef ROLE_CTRL
#define ROLE_CTRL 40
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

// Timer to MASTER TAG
#ifndef TIMER_CTRL_TAG
#define TIMER_CTRL_TAG 40
#endif

void actorCode(int initN, int Ncell, int maxN, int initInfection, int timeAll);

#endif
