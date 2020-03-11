#include "../include/master.h"

void masterSimulationInit(int initN, int Ncell, int initInfection,
                          int timeAll, int maxN) {
  long seed = 20;
  printf("Simulation started\ninitN = %d\n", initN);
	initialiseRNG(&seed);
  startTimer(timeAll); // consider void
	startLand(initN, initInfection); // consider void
	startCTRL(maxN);
  startSquirrel(initN, initInfection, seed);

}

void startSquirrel(int initN, int initInfection, long seed) {
  int i, sID, msg, isInfected;
  int Ninf = initInfection;
  printf("Initialise %d 🐿️\n", initN);
  for (i = 0; i < initN; i++) {

    sID = startWorkerProcess();
    // actorSEND(ROLE_SQUIRREL, sID, ROLE_TAG);
    // Send role
    msg = ROLE_SQUIRREL;
    MPI_Bsend(&msg, 1, MPI_INT, sID, ROLE_TAG, MPI_COMM_WORLD);

    // Send infection status
    if (Ninf > 0) {
      isInfected = 1;
      Ninf--;
    } else
      isInfected = 0;
		MPI_Bsend(&isInfected, 1, MPI_INT, sID, INF_TAG, MPI_COMM_WORLD);
    // actorSEND(isInfected, sID, INF_TAG);
    //
    // Initialise and send born location
    float bornPOS[2] = {0, 0};
    float diff=ran2(&seed);
    bornPOS[0] = (bornPOS[0]+diff)-(int)(bornPOS[0]+diff);
    diff=ran2(&seed);
    bornPOS[1] = (bornPOS[1]+diff)-(int)(bornPOS[1]+diff);
		// sleep(1);
    MPI_Bsend(bornPOS, 2, MPI_FLOAT, sID, POS_TAG, MPI_COMM_WORLD);
  }
	return;
}

// TODO : 16 is a problem
void startLand(int initN, int initInfection) {
	int lID, msg, i;
	for (i = 0; i<16; i++) {
		lID = startWorkerProcess();
		// Send role
		msg = ROLE_LAND;
		MPI_Bsend(&msg, 1, MPI_INT, lID, ROLE_TAG, MPI_COMM_WORLD);
	}

	return;
}

void startTimer(int timeAll) {
  int tID, msg;
  tID = startWorkerProcess();
	// Send role
	msg = ROLE_TIMER;
	MPI_Bsend(&msg, 1, MPI_INT, tID, ROLE_TAG, MPI_COMM_WORLD);
}

void startCTRL(int maxN) {
	int CID, msg;
	CID = startWorkerProcess();
	// Send role
	msg = ROLE_CTRL;
	MPI_Bsend(&msg, 1, MPI_INT, CID, ROLE_TAG, MPI_COMM_WORLD);
}

// void masterTerminationCtrl(int maxN) {
//
// 	int pop = 0, cnt = 0;
// 	int masterStatus = masterPoll();
	// // Used as a block to seek for finalize comditions
	// while (masterStatus) {
	//   printf("Waiting termination signal\n");
	// 	int isAlive, flag;
	// 	MPI_Status status;
	//
	// 	// Population control
	// 	MPI_Iprobe(MPI_ANY_SOURCE, TIMER_CTRL_TAG, MPI_COMM_WORLD, &flag, &status);
	// 	if (flag) {
	// 		MPI_Recv(&isAlive, 1, MPI_INT, TIMER_ID, TIMER_CTRL_TAG,
	// 						 MPI_COMM_WORLD, &status);
	// 		sleep(1);
	// 		printf("\tFINISHED: Simulation Stoped. Population: %d\n", pop);
	// 		shutdownPool();
	// 		return;
	// 	}
	//
	// 	MPI_Iprobe(MPI_ANY_SOURCE, POP_CTRL_TAG, MPI_COMM_WORLD, &flag, &status);
	// 	if (flag) {
	// 		MPI_Recv(&isAlive, 1, MPI_INT, status.MPI_SOURCE, POP_CTRL_TAG,
	// 						 MPI_COMM_WORLD, &status);
	// 	}
	//
	// 	if (isAlive == 1) {
	// 		pop++;
	// 		printf("\t\tmaster receive birth[V] signal from squirrel %d. pop: %d\n", status.MPI_SOURCE, pop);
	// 	}
	//
	// 	else if (isAlive == 0) {
	// 		pop--;
	// 		printf("\t\tmaster receive death[X] signal from squirrel %d. pop: %d\n", status.MPI_SOURCE, pop);
	// 	}
	// 	if (pop >= maxN) {
	// 		printf(
	// 				"\tERROR: Simulation Stoped (maximal allowed population reached)\n");
	// 		break;
	// 	}
	//
	// 	masterStatus = masterPoll();
	// }
// 	return;
// }
