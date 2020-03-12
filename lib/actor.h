#ifndef ACTOR_H
#define ACTOR_H


void actorRun(void (*roleRun)(int, int), int maxN, int timeAll);

int actorGetID(void);

int actorCreate(void);

void actorSendMsg(int msg, int targetID, int tag);

int actorGetCreatorID(void);

int actorStop(void);

int actorDie(void);

int actorRecv(int sourceID, int tag);

#endif