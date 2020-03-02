#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#ifndef ROLE_TIMER
#define ROLE_TIMER -1
#endif
void actorCode(int *role);
void timerCode();
