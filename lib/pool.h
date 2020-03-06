#ifndef POOL_H_
#define POOL_H_

// The core process pool command which instructs what to do next
enum PP_Control_Command {
	PP_STOP=0,
	PP_SLEEPING=1,
	PP_WAKE=2,
	PP_STARTPROCESS=3,
	PP_RUNCOMPLETE=4
};

// An example data package which combines the command with some optional data, an example and can be extended
struct PP_Control_Package {
	enum PP_Control_Command command;
    int data;
};

// Initialises the process pool
int processPoolInit();
// Finalises the process pool
void processPoolFinalise();
// Called by the master in loop, blocks until state change, 1=continue and 0=stop
int masterPoll();
// Called by worker once task completed and will sleep until it receives instructions, 1=continue with new task and 0=stop
int workerSleep();
// Determines whether the current worker should stop or not (i.e. whether the pool is shutting down)
int shouldWorkerStop();
// Called by the master or a worker to start a new worker process
int startWorkerProcess();
// Called by a worker to shut the pool down
void shutdownPool();
// Retrieves the optional data associated with the command, provides an example of how this can be done
int getCommandData();

#endif /* POOL_H_ */
