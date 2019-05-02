
#include <stdio.h>
#include <mpi.h>
#include <string.h>

int main(int argc, char* argv[]) {
    int numprocs, rank;
    MPI_Status status;
    char processor_name[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    const int SIZE = numprocs * 4;
    char sendString[10];
    char receivedString[10];

     if (rank== 0) {
          //send a message containing its rank to the process with rank 1
        MPI_Send(sendString, strlen(sendString) + 1, MPI_CHAR, 1, 1, MPI_COMM_WORLD);
        // receive a message from the process  with rank n - 1
        MPI_Recv(receivedString, SIZE, MPI_CHAR, numprocs - 1, 1, MPI_COMM_WORLD, &status);
         // print the message to the screen
         printf("Process %d on %s recieved message from process %s\n", rank, processor_name, receivedString);
     }
    else{ // rank > 0
       //receive a message from the process with rank  i - 1,
        MPI_Recv(receivedString, SIZE, MPI_CHAR, rank - 1, 1, MPI_COMM_WORLD, &status);
        //verify that the message contains the integer i - 1,
        // replace the integer i - 1 with the integer i,
        sprintf(sendString, "%s%d ", receivedString, rank);
        //send the modified message to the process with rank i+1. 
        MPI_Send(sendString, strlen(sendString) + 1, MPI_CHAR, (rank + 1) % numprocs, 1, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}