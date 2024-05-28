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

7. Page 17: **`MPI_Bcast(address to get the value, # elements, data type, rank, MPI_COMM_WORLD)`**

8. Page 20: Sending Data from One Source CPU to Just One Destination CPU.<br>
   **`MPI_Send(address to get the data, # elements, data type, rank of the destination, tag to differentiate from any other transmission, MPI_COMM_WORLD)`**

9. Page 21: Receiving Data in a Destination CPU from a Source CPU.<br>
   **`MPI_Recv(address to store the data, # elements, data type, rank of the source, tag to differentiate differentiate what transmission we are looking for with this call, MPI_COMM_WORLD, &status)`**

10. Page 22: **MPI discourages you from sending to yourself!** <br>Because both the send and receive are capable of blocking til they happen, the result could be ***deadlock***!

11. Page 23: There is a Transmission Buffer to store data until `MPI_Recv` is called. This is how it does not have to synchronize really.

12. Page 24: A Better Example and more realistic way of MPI sending and receiving.

13. Page 25: Design Patterns: `Scatter`, `Gather`, and `Broadcast``.<br><br>
   ***Scatter and Gather Usually Go Together.***

14. Page 27: **MPI_Scatter()** is like broadcast. It's detailed descriptions.

15. Page 28: **MPI_Gather()**'s detailed descriptions.

16. 
     
