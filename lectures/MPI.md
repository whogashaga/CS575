## Message Passing Interface (MPI)

### mpi.pptx

1. Basic Idea: Programs on different CPUs coordinate computations by passing messages between each other.
   <br> **Note**: Each MPI CPU must also have an integer ID assigned to it (called its ***rank***).

2. Page 7: **Do not** use ***`g++`*** command.<br>
   Use ***`mpic++`*** to compile, e.g. `mpic++ -o program program.cpp ...`<br>
   Then ***`mpiexec`*** to run, e.g. `mpiexec -mca btl self,tcp -np 4 program`

3. Page 8: The bash script, which is similar to DGX.

4. Page 10: `--main-user=user@oregonstate.edu`

5. Page 11: Cancel the job.

6. Page 13: A ***Communicator*** is a collection of CPUs that are capable of sending messages to each other.<be>
   total number of CPUs involved, **Size**, e.g. `MPI_Comm_size( MPI_COMM_WORLD, &numCPUs );`<br>
   which one I am, **Rank**, e.g.`MPI_Comm_rank( MPI_COMM_WORLD, &me );`

7. 
