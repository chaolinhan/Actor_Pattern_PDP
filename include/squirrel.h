#ifndef SQUIRREL_H
#define SQUIRREL_H

#include "string.h"
#include "stdio.h"
#include "actorCode.h"
#include "../lib/pool.h"
#include "../lib/ran2.h"
#include "../lib/squirrel-functions.h"

void squirrelRUN(int initN, int Ncell, int maxN, int initInfection, int timeAll);
#endif
