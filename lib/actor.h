#ifndef ACTOR_H
#define ACTOR_H

#include "pool.h"
#include "ran2.h"
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>


void actorRUN(void (*roleRUN)(int, int, int, int), int initN, int Ncell, int maxN, int initInfection);

#endif
