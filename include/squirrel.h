#ifndef SQUIRREL_H
#define SQUIRREL_H

// Run squirrel actor's code
void squirrelRUN(int maxN, int timeAll);

// Get MPI rank from cell ID
static int getLandID(int cellID);

#endif

