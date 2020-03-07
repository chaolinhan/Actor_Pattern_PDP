TESTSRC = \
  src/test.c \
  lib/pool.c
SRC = \
  src/model.c \
	src/utility.c \
	src/actorCode.c \
	src/master.c \
	lib/pool.c \
	lib/squirrel-functions.c \
	lib/ran2.c \
	lib/actor.c \
	role/squirrel.c \
	role/land.c \
	role/timer.c

LFLAGS = -lm -O2
CC=mpicc

all:
	$(CC) $(SRC) -o model $(LFLAGS)
test:
	$(CC) $(TESTSRC) -o $@ $(LFLAGS)

.PHONY : clean
clean:
	rm -f test
	rm -f model
