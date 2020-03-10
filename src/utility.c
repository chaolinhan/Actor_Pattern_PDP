#include "../include/utility.h"

int inputCheck(int argc) {
  if (argc == 2)
    return 1;
  else
    return 0;
  // TODO: inputs range check
}

void readPara(int *initN, int *Ncell, int *maxN, int *initInfection,
              int *timeAll, char *argv[]) {
	FILE *in;
  in = fopen(argv[1], "r+");
  char s[20];
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  fscanf(in, "%s%d\ncells:%d\nmax:%d\ninfected:%d\nmonth:%d", s, initN, Ncell,
         maxN, initInfection, timeAll);
  if (rank == 0) {
    printf("Parameters read\n%d\t%d\t%d\t%d\t%d\n", *initN, *Ncell, *maxN,
           *initInfection, *timeAll);
  }

  return;
}
