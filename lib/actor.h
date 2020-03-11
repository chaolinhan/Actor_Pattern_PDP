#ifndef ACTOR_H
#define ACTOR_H


/**
 * Initiate a function to run specific actor code.
 * @param roleRun pointing to the function to be executed.
 * Rest parameters are supplied to the pointed function
 */
void actorRun(void (*roleRun)(int, int), int maxN, int timeAll);

/**
 * Get the ID of a actor. An ID is unique at any time points.
 * @return ID
 */
int actorGetID(void);

/**
 * Create a new actor.
 * @return the ID of created actor.
 */
int actorCreate(void);

void actorSendMsg(int msg, int targetID, int tag);

int actorGetCreatorID(void);

int actorStop(void);

int actorDie(void);

int actorRecv(int sourceID, int tag);

#endif