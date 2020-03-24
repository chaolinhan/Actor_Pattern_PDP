#ifndef ACTOR_H
#define ACTOR_H

#include <mpi.h>
#ifndef ANY_SOURCE
#define ANY_SOURCE MPI_ANY_SOURCE
#endif

/**
 * Used to stored received message
 * msg: message content
 * src: sender ID
 */
struct actorMSG {
	int msg;
	int src;
};

// Initiate MPI and process poll
int actorInit(int argc, char *argv[]);

// Finalise MPI and process poll
void actorExit(int type);

// Initiate a function 'roleRun()' to run specific actor role code
void actorRun(void (*roleRun)(int, int), int maxN, int timeAll);

// Get the ID of a actor. An ID is unique at any time points
int actorGetID(void);

// Create a new actor
int actorCreate(void);

// Send message to other actor
void actorSend(int msg, int targetID, int tag);

// Get the creator's ID
int actorGetCreatorID(void);

// Test if actor should stop acting
int actorStop(void);

// Destroy the actor
int actorDie(void);

// Receive message from other actors
struct actorMSG actorRecv(int sourceID, int tag);

// Listen from other actors and find if there is a message to be received
int actorProbe(int sourceID, int tag);

#endif