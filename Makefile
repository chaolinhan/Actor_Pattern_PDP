TESTSRC = \
  src/test.c \
  lib/pool.c
SRC = \
  src/main.c \
	src/actorCode.c \
	src/master.c \
	src/utility.c \
	lib/pool.c \
	lib/squirrel-functions.c \
	lib/ran2.c \
	lib/actor.c \
	role/squirrel.c \
	role/land.c \
	role/timer.c \
	role/ctrl.c

LFLAGS = -lm -O2
CC=mpicc

all:
	$(CC) $(SRC) -o model $(LFLAGS)
test:
	$(CC) $(TESTSRC) -o $@ $(LFLAGS)

run:
	mpiexec_mpt -ppn 36 -n 216 ./model para.txt

.PHONY : clean
clean:
	rm -f test
	rm -f model
