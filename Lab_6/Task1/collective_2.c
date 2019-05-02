#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <sys/time.h> 
#define N 10

int main (int argc, char* argv[]){

  int i, rank, numtasks, element;
  int array[N],rootarray[N];
  int *receivedarray;
  int *sendchunk, *chunk;
  double time_taken;
  struct timeval start, end;
  //time
  gettimeofday(&start, NULL);
  MPI_Init(&argc, &argv); 
  MPI_Comm_rank (MPI_COMM_WORLD, &rank); 
  MPI_Comm_size (MPI_COMM_WORLD, &numtasks); 

  // initializes a one-dimensional array assigning to each element the value of its index.
    if(rank == 0) 
    {
      for(i=0;i<N;i++){
        array[i]=i ;
      }
    }
    element = N/numtasks;
    if(rank < (N%numtasks))
    {
      element += element;
    }
    receivedarray = (int*) malloc(element*sizeof(int));

    printf("element value = %d rank = %d\n",element,rank);
    //MPI_scatter/gather
    if((N%numtasks) == 0 )
    {
      MPI_Scatter(array,element, MPI_INT,receivedarray,element, MPI_INT, 0, MPI_COMM_WORLD);
      // sent to other processes
      for(i=0;i<element;i++){
        printf("rank = %d , Received elements : %d\n", rank,receivedarray[i]);
        receivedarray[i]= receivedarray[i]+rank;
      }
      MPI_Gather(receivedarray,element,MPI_INT,rootarray,element,MPI_INT,0,MPI_COMM_WORLD);
      if(rank == 0)
      {
        for(i=0;i<N;i++){
          printf("i = %d root array : %d\n", i,rootarray[i]);
        }
      }
    }
    //After  having  received  the  proper chunk
    //repeat process from first
    // each process updates it by adding its rank 
    else{
      sendchunk = (int*) malloc(numtasks*sizeof(int));
      chunk = (int*) malloc(numtasks*sizeof(int));

      if(rank == 0)
      {
        //MPI_scatterv/gatherv
          MPI_Scatterv(array,sendchunk,chunk,MPI_INT, receivedarray,element, MPI_INT, 0, MPI_COMM_WORLD);
          //sends it back to root process. 
          for(i=0;i<element;i++){
           printf("rank = %d, Received elements: %d\n ", rank,receivedarray[i]);
            receivedarray[i]= receivedarray[i]+rank;
          }
          printf("\n");
          MPI_Gatherv(receivedarray,element,MPI_INT,rootarray,sendchunk,chunk,MPI_INT,0,MPI_COMM_WORLD);
          if(rank == 0){
            printf("root array : ");
              for(i=0;i<N;i++){
                printf("%d \t",rootarray[i]);
              }
          }
          free(sendchunk);
          free(chunk);
      }
    }
    free(receivedarray);
    //time
    gettimeofday(&end, NULL); 
    time_taken = (end.tv_sec - start.tv_sec) * 1e6; 
    time_taken = (time_taken + (end.tv_usec - start.tv_usec)) * 1e-6;
    printf( "Total time = %f seconds \n" , time_taken); 
    
    MPI_Finalize();
    return 0;
}