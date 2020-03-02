TESTSRC = \
  src/test.c \
  src/pool.c
SRC = \
  src/model.c \
	src/utility.c \
	src/pool.c \
	src/actor.c
LFLAGS =-lm
CC=mpicc

all:
	$(CC) $(SRC) -o model $(LFLAGS)
test:
	$(CC) $(TESTSRC) -o $@ $(LFLAGS)

.PHONY : clean
clean:
	rm -f test
	rm -f model
