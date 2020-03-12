#include "actor.h"
#include "pool.h"
#include "ran2.h"
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Initiate a function to run specific actor code.
 * @param roleRun pointing to the function to be executed.
 * Rest parameters are supplied to the pointed function
 */
void actorRun(void (*roleRun)(int, int), int maxN, int timeAll) {
	roleRun(maxN, timeAll);
}

/**
 * Get the ID of a actor. An ID is unique at any time points.
 * @return ID
 */
int actorGetID(void) {
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	return rank;
}

int actorCreate(void) {
	int pid;
	pid = startWorkerProcess();
	return pid;
}

void actorSendMsg(int msg, int targetID, int tag) {
	MPI_Bsend(&msg, 1, MPI_INT, targetID, tag, MPI_COMM_WORLD);
}

int actorGetCreatorID(void) {
	return getCommandData();
}

int actorStop(void) {
	return shouldWorkerStop();
}

int actorDie(void) {
	return workerSleep();
}

struct actorMSG actorRecv(int sourceID, int tag) {
	int msg = -1;
	MPI_Status st;
	struct actorMSG msgReturn = {-1,-1};
	if (actorStop()) return msgReturn;
	MPI_Recv(&msg, 1, MPI_INT, sourceID, tag, MPI_COMM_WORLD, &st);
	msgReturn.src = st.MPI_SOURCE;
	msgReturn.msg = msg;
	return msgReturn;
}

int actorProbe(int sourceID, int tag) {
	int flag;
	MPI_Status st;
	MPI_Iprobe(sourceID, tag, MPI_COMM_WORLD, &flag, &st);
	return flag;
}
int actorInit(int argc, char *argv[]) {
	MPI_Init(&argc, &argv);
	return processPoolInit();;
}
void actorExit(int type) {
	if (type == 0) {
		int rank = actorGetID();
		if (rank == 0) exit(0);
	}
	processPoolFinalise();
	MPI_Finalize();
}
