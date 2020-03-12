#ifndef ACTOR_H
#define ACTOR_H

#include <mpi.h>
#ifndef ANY_SOURCE
#define ANY_SOURCE MPI_ANY_SOURCE
#endif

struct actorMSG {
	int msg;
	int src;
};

int actorInit(int argc, char *argv[]);

void actorExit(int type);

void actorRun(void (*roleRun)(int, int), int maxN, int timeAll);

int actorGetID(void);

int actorCreate(void);

void actorSendMsg(int msg, int targetID, int tag);

int actorGetCreatorID(void);

int actorStop(void);

int actorDie(void);

struct actorMSG actorRecv(int sourceID, int tag);

int actorProbe(int sourceID, int tag);

#endif