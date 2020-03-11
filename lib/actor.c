#include "actor.h"
#include "pool.h"
#include "ran2.h"
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

void actorRUN(void (*roleRUN)(int, int, int, int, int), int initN, int Ncell, int maxN, int initInfection, int timeAll) {
	roleRUN(initN, Ncell, maxN, initInfection, timeAll);
}
int actorGetID(void) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  return rank;
}
