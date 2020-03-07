#include "timer.h"

void timerRUN(int initN, int Ncell, int maxN, int initInfection, int timeAll) {
  MPI_Status status;
  int rank, month, ii;
  double tStart, tNow;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  printf("Timer started\n");
  month = 0;
  tStart = MPI_Wtime();
  while (month <= timeAll) {
    tNow = MPI_Wtime();
		// Month change every two seconds
    if ((int)(tNow - tStart) / 2 != month) {
      month++;
      printf("Timer report: month %d\n", month);

			// Send month to Land
			for (ii = 2; ii<=17; ii++) {
				MPI_Bsend(&month, 1, MPI_INT, ii, MONTH_TAG, MPI_COMM_WORLD);
			}
    }

		if(shouldWorkerStop()) break;

  }
  return;
}
