# Actor pattern, created in Parallel Design Pattern Coursework, UoE
Squirrel infection simulation using actor pattern.

# Actor Framework
To implement actor pattern on the biological model, a framework is written to provide functions related to actor behaviours, and then it is used to code different types of actors. The implementation aims to wrap process pool and MPI code into actor pattern functions so that a user could implement the actor pattern to other models by directly using the framework rather than code using MPI and process pool. However in this biological model MPI functions are still needed to obtain the wall time and pass irregular messages across actors.

## Framework

Functions provided in the framework are shown in Figure 1. They are implemented using MPI and provided process pool functions. Together with process pool code (pool.c and pool.h) and random number generator (ran2.c and ran2.h) they are regarded as library to code the actor pattern.

The aim as stated before is to avoid users directly touching MPI and process pool functions. The framework functions should not be problem specific, but unfortunately as they are all coded in C rather than C++, one function actorRun() uses function pointer to trigger actions of different kinds of actors by passing two problem-specific parameters (total simulation time and max number of squirrels), thus this function is required to changed before applying it in other context. One possible way to avoid this situation is to use void function pointer and passing no parameters; the required parameters is passed using MPI send and receive instead. However this could introduce more complexity and more message to be passed between actors which might affect the performance.

The functions in actor.c are more likely a wrap-up of MPI and process pool functions. User should only know that the actor pattern is implemented as one actor per MPI process with a master, and how to use these functions; they should not know the underlying master-worker process pool.

actorSend(), actorRecv() and actorProbe() provide simple and easy message passing functions. They simplify MPI send/recv into less parameters (message, target and message tag). actorProbe is used to probe incoming massages, which is necessary regarding the asynchrony nature of the actor pattern in this case. The detailed implementation of message passing is denoted in the next section.

The only exception that from MPI message passing functions which is not included as a framework function is non-integer message passing, which will be explained in section 2.2.1 in detail.

main.c provides an example of the start the simulation. The actor pattern is applied under master-worker pattern, where actors are running their customised actorCode() and master should initialise actors (masterSimulationInit()) and wait until the simulation stops (masterWait()). This execution framework applies to other model implementations.

The design of actor pattern is based on the squirrels problem but efforts have been made to ensure the generalisation of framework functions. One example is the city traffic simulation problem where this framework could be used to implement actor pattern. After proper initialisation, cars, roads, traffic lights and other objects can all be represented by different types of actor, using actorRun() to perform their own dynamics, in which actorSend/Probe/Recv and actorGetID/GetCreatorID could be used to communicate with other actors; actorCreate/Die could be used to simulate the dynamical creation and elimination of actors e.g. cars retire, road close and new car coming out on road.

![](https://i.imgur.com/ugav5yC.png)

## Communication pattern

![](https://i.imgur.com/bYHrH71.png)

## Process poll functions

![](https://i.imgur.com/LXQwJrZ.png)

# Getting started
- [Overview](#Overview)
- [Build and Run](#Build-and-Run)
- [Notes on code](#notes-on-code)

# Overview
This is a program that implements actor pattern to solve squirrel infection dynamics. Actor pattern code and provided squirrel functions are included in `lib/`.
## File structure
```
.
├── para.txt // input parameter
├── Makefile
├── READ.md
├── include
│   └── // include path
├── lib
│   └── // framework library path
├── role
│   └── // source code – actor roles
└── src
    └── // source code
```
## Test environment
On **Cirrus** (CentOS Linux 7 (Core)):

GNU Make `make` version 3.82

MPI: HPE MPT (Message Passing Toolkit) 2.18

PBS version 14.2.7.20190903110743

# Build and Run
`Makefile` is included to build the code. [GNU Make](https://www.gnu.org/software/make/) and MPI compiler are required.

Before any building:
```
make clean
```
## Build
```
make
```
The output file `model` is an executable. To run the simulation, fill your parameter settings in `para.txt` after the colon of each line:
```
initial: 34
cells: 16
max: 200
infected: 4
month: 10
```
## Run the simulation
```
mpiexec_mpt -ppn 36 -n 216 ./model para.txt
```
The flags are: `-n [total number of MPI processes]` and `-ppn [parallel processes per node]` which should be changed accordingly. In this case 6 nodes are used with 216 parallel process in total (36 processes per core). You can simply use `make run` to execute the code above under default settings (`-ppn 36 -n 216`).

More information on `mpiexec_mpt` please see [here](https://cirrus.readthedocs.io/en/master/user-guide/batch.html#hpe-mpt-parallel-launcher-mpiexec-mpt).

On a PBS system `simulation_Cirrus.pbs` can be submitted to run the simulation:
```
qsub simulation_Cirrus.pbs
```
`simulation_Cirrus.pbs` may be modified accordingly (e.g. user, working directories etc.) to be submitted.

# Notes on code
The code basically follows lower camel case in variables, functions and structures. The coding style is [Google style](https://google.github.io/styleguide/cppguide.html).