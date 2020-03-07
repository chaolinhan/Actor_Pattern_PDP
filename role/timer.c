#include "timer.h"

void timerRUN(int initN, int Ncell, int maxN, int initInfection, int timeAll) {
  MPI_Status status;
  int rank, month;
  double tStart, tNow;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  printf("Timer started\n");
  month = 0;
  tStart = MPI_Wtime();
  while (month < timeAll) {
    tNow = MPI_Wtime();
    if ((int)(tNow - tStart) / 2 != month) {
      month++;
      printf("Timer report: month %d\n", month);
    }

  }

}
