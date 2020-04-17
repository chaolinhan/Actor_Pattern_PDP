# Parallel Design Pattern coursework Part Two
Squirrel infection simulation using actor pattern.
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