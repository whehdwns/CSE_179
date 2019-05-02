#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <sys/time.h> 
int main (int argc, char* argv[]){

  int rank, numtasks;
  int initalize_var;
  int modified_var;
  double time_taken;
  struct timeval start, end;
  gettimeofday(&start, NULL);
  MPI_Init(&argc, &argv);	
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);	
  MPI_Comm_size (MPI_COMM_WORLD, &numtasks);	

  //MPI  process  0 initializes  a  variable  to  a  given  value
  if(rank == 0) {
    initalize_var=8;
    printf("Initalize variable is :%d\n", initalize_var);
    //calculating the square of its value
    modified_var = initalize_var*initalize_var ;
  }
  else {
    initalize_var=0;
    modified_var=0;
  }

  MPI_Bcast(&modified_var, 1, MPI_INT, 0, MPI_COMM_WORLD);
  gettimeofday(&end, NULL); 
  time_taken = (end.tv_sec - start.tv_sec) * 1e6; 
  time_taken = (time_taken + (end.tv_usec - start.tv_usec)) * 1e-6; 
  printf("rank is %d  initalized variable is %d and modified variable is %d\n", rank,initalize_var,modified_var);
  printf( "Total time = %f seconds \n" , time_taken);
  MPI_Finalize();
  return 0;
}