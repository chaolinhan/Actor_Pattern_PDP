#include "actor.h"
#include "pool.h"
#include "mpi.h"
#include <stdlib.h>

/**
 * Initiate a function to run specific actor code.
 * @param roleRun pointing to the function to be executed
 * Rest parameters are supplied to the pointed function
 */
void actorRun(void (*roleRun)(int, int), int maxN, int timeAll) {
	roleRun(maxN, timeAll);
}

/**
 * Get the ID of a actor. An ID is unique at any time points
 * @return ID
 */
int actorGetID(void) {
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	return rank;
}

/**
 * Create a new actor
 * @return the ID of the new actor
 */
int actorCreate(void) {
	int pid;
	pid = startWorkerProcess();
	return pid;
}

/**
 * Send message to other actor
 * @param msg content
 * @param targetID receiver's ID
 * @param tag message tag to prevent mess
 */
void actorSend(int msg, int targetID, int tag) {
	MPI_Bsend(&msg, 1, MPI_INT, targetID, tag, MPI_COMM_WORLD);
}

/**
 * Get the actor's parent's ID
 * @return parent's ID
 */
int actorGetCreatorID(void) {
	return getCommandData();
}

/**
 * Test if actor should stop acting
 * @return 1: stop
 *         0: no need to stop
 */
int actorStop(void) {
	return shouldWorkerStop();
}

/**
 * Destroy actor. The process could be awaken by assigning other actor roles on it
 */
int actorDie(void) {
	return workerSleep();
}

/**
 * Receive message from other actors
 * @param sourceID the sender's ID; use ANY_SOURCE to receive from anyone
 * @param tag message tag; use ANY_TAG to receive messages with any tag
 * @return struct actorMSG containing sender ID and message content
 */
struct actorMSG actorRecv(int sourceID, int tag) {
	int msg = -1;
	MPI_Status st;
	struct actorMSG msgReturn = {-1, -1};
	if (actorStop()) return msgReturn;
	MPI_Recv(&msg, 1, MPI_INT, sourceID, tag, MPI_COMM_WORLD, &st);
	msgReturn.src = st.MPI_SOURCE;
	msgReturn.msg = msg;
	return msgReturn;
}

/**
 * Listen from other actors and find if there is a message to be received
 * @param sourceID the sender's ID; use ANY_SOURCE to listen from anyone
 * @param tag message tag
 * @return 1: message to be received
 *         0: mo message to be received
 */
int actorProbe(int sourceID, int tag) {
	int flag;
	MPI_Status st;
	MPI_Iprobe(sourceID, tag, MPI_COMM_WORLD, &flag, &st);
	return flag;
}

/**
 * Initiate MPI and process poll
 * @return the status code indicating master or worker
 */
int actorInit(int argc, char *argv[]) {
	MPI_Init(&argc, &argv);
	return processPoolInit();;
}

/**
 * Finalise MPI and process poll
 * @param 0: force quite after the simulation stops
 *        other value: manual control quite
 */
void actorExit(int type) {
	if (type == 0) {
		int rank = actorGetID();
		if (rank == 0) exit(0);
	}
	processPoolFinalise();
	MPI_Finalize();
}

/**
 * Stop all other actors
 */
void actorAllStop(void) {
	shutdownPool();
}

/**
 * Check actor and decide should stop wait or not
 * @return 1 if should wait
 *         other values if should not wait
 */
int masterCheckActor(void) {
	return masterPoll();
}
