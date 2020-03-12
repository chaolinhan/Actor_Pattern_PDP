#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ran2.h"
#include "squirrel-functions.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/**
 * Initialises the random number generator, call it once at the start of the program on each process. The input
 * value should be negative, non-zero and different on each process, I suggest something like -1-rank where
 * rank is the MPI rank.
 * ran2 is a flawed RNG and must be used with particular care in parallel however it is perfectly sufficient
 * for this course work
 * The seed is modified and then passed to each other subsequent function that relies on the random generator,
 * which also modify the seed
 */
void initialiseRNG(long *seed) {
	ran2(seed);
}

/**
 * Simulates the step of a squirrel. You can call this with the arguments (0,0,&x,&y,&state)
 * to determine a random initial starting point.
 * x_new and y_new are the new x and y coordinates, state is used in the random number
 * generator and is also modified.
 * x_new can point to x, and y_new can point to y
 */
void squirrelStep(float x, float y, float *x_new, float *y_new, long *state) {

	float diff = ran2(state);
	*x_new = (x + diff) - (int) (x + diff);

	diff = ran2(state);
	*y_new = (y + diff) - (int) (y + diff);
}

/**
 * Determines whether a squirrel will give birth or not based upon the average population and a random seed
 * which is modified. You can enclose this function call in an if statement if that is useful.
 */
int willGiveBirth(float avg_pop, long *state) {
	float probability = 100.0; // Decrease this to make more likely, increase less likely
	float tmp = avg_pop / probability;

	return (ran2(state) < (atan(tmp * tmp) / (4 * tmp)));
}

/**
 * Determines whether a squirrel will catch the disease or not based upon the average infection level
 * and a random seed which is modified. You can enclose this function call in an if statement if that is useful.
 */
int willCatchDisease(float avg_inf_level, long *state) {
	float probability = 1000.0; // Decrease this to make more likely, increase less likely
	return (ran2(state) < (atan(((avg_inf_level < 40000 ? avg_inf_level : 40000)) / probability) / M_PI));
}

/**
 * Determines if a squirrel will die or not. The state is used in the random number generation and
 * is modified. You can enclose this function call in an if statement if that is useful.
 */
int willDie(long *state) {
	return (ran2(state) < (0.166666666));
}

/**
 * Returns the id of the cell from its x and y coordinates.
 */
int getCellFromPosition(float x, float y) {
	return ((int) (x * 4) + 4 * (int) (y * 4));
}
