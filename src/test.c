/*
 * Example code to run and test the process pool. To compile use something like mpicc -o test test.c pool.c
 */

#include <stdio.h>
#include "mpi.h"
#include "pool.h"

static void workerCode();

int main(int argc, char* argv[]) {
	// Call MPI initialize first
	MPI_Init(&argc, &argv);
	/*
	 * Initialise the process pool.
     * The return code is = 1 for worker to do some work, 0 for do nothing and stop and 2 for this is the master so call master poll
     * For workers this subroutine will block until the master has woken it up to do some work
	 */
	int statusCode = processPoolInit();
	if (statusCode == 1) {
		// A worker so do the worker tasks
		workerCode();
	} else if (statusCode == 2) {
		/*
		 * This is the master, each call to master poll will block until a message is received and then will handle it and return
         * 1 to continue polling and running the pool and 0 to quit.
         * Basically it just starts 10 workers and then registers when each one has completed. When they have all completed it
         * shuts the entire pool down
		 */
		int i, activeWorkers=0, returnCode;
		MPI_Request initialWorkerRequests[10];
		for (i=0;i<10;i++) {
			int workerPid = startWorkerProcess();
			MPI_Irecv(NULL, 0, MPI_INT, workerPid, 0, MPI_COMM_WORLD, &initialWorkerRequests[i]);
			activeWorkers++;
			printf("Master started worker %d on MPI process %d\n", i , workerPid);
		}
		int masterStatus = masterPoll();
		while (masterStatus) {
			masterStatus=masterPoll();
			for (i=0;i<10;i++) {
				// Checks all outstanding workers that master spawned to see if they have completed
				if (initialWorkerRequests[i] != MPI_REQUEST_NULL) {
					MPI_Test(&initialWorkerRequests[i], &returnCode, MPI_STATUS_IGNORE);
					if (returnCode) activeWorkers--;
				}
			}
			// If we have no more active workers then quit poll loop which will effectively shut the pool down when  processPoolFinalise is called
			if (activeWorkers==0) break;
		}
	}
	// Finalizes the process pool, call this before closing down MPI
	processPoolFinalise();
	// Finalize MPI, ensure you have closed the process pool first
	MPI_Finalize();
	return 0;
}

static void workerCode() {
	int workerStatus = 1;
	while (workerStatus) {
		int myRank, parentId;
		MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
		parentId = getCommandData();	// We encode the parent ID in the wake up command data
		if (parentId == 0) {
			// If my parent is the master (one of the 10 that the master started) then spawn two further children
			int childOnePid = startWorkerProcess();
			int childTwoPid = startWorkerProcess();

			printf("Worker on process %d, started two child processes (%d and %d)\n", myRank, childOnePid, childTwoPid);
			// Wait for these children to send me a message
			MPI_Request childRequests[2];
			MPI_Irecv(NULL, 0, MPI_INT, childOnePid, 0, MPI_COMM_WORLD, &childRequests[0]);
			MPI_Irecv(NULL, 0, MPI_INT, childTwoPid, 0, MPI_COMM_WORLD, &childRequests[1]);
			MPI_Waitall(2, childRequests, MPI_STATUS_IGNORE);
			// Now tell the master that I am finished
			MPI_Send(NULL, 0, MPI_INT, 0, 0, MPI_COMM_WORLD);
		} else {
			printf("Child worker on process %d started with parent %d\n", myRank, parentId);
			// Tell my parent that I have been alive and am about to die
			MPI_Send(NULL, 0, MPI_INT, parentId, 0, MPI_COMM_WORLD);
		}

		workerStatus=workerSleep();	// This MPI process will sleep, further workers may be run on this process now
	}
}
