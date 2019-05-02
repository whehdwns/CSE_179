#include <mpi.h>
#include <stdio.h>
#include <sys/time.h> 
#define n 4   //matrix  size  n as  input

int A[n][n], B[n][n]; //  generate  two  random  nxn matrices
int C[n][n];

int main(int argc, char** argv)
{
  int rank, q, num, i, j, k;
  int jj, kk, temp;
  double time_taken;
  struct timeval start, end;
  MPI_Status status;

  gettimeofday(&start, NULL); // time
  MPI_Init (&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
  MPI_Comm_size(MPI_COMM_WORLD, &q); 

  num = rank* n/q; //n is always divisible  by  q .

  for (i=0; i<n; i++){
    for (j=0; j<n; j++){
        //A[i][j]= i+j;
        A[i][j] = rand() % 10; //randomly generate the number of the matrix A
      }
  }

  for (i=0; i<n; i++){
    for (j=0; j<n; j++){
        //B[i][j] = i+j;
       B[i][j] = rand() % 10; //randomly generate the number of the matrix B
      }
  }


  MPI_Scatter (A, n*n/q, MPI_INT, A[num], n*n/q, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Scatter (B, n*n/q, MPI_INT, B[num], n*n/q, MPI_INT, 0, MPI_COMM_WORLD);
  //normal matrix multiplication
  for (i=num; i<n; i++) {
    for (j=0; j<n; j++) {
      C[i][j]=0;
      for (k=0; k<n; k++){
        C[i][j] += A[i][k]*B[k][j];
      }
    }
  }
  
  //matrix of the serial version
  //Block_mat_mult
  /*for(jj=0;jj<n;jj+= q){
      for(kk=0;kk<n;kk+= q){
              for(i=0;i<n;i++){
                      for(j = jj; j<((jj+q)>n?n:(jj+q)); j++){
                              temp = 0;
                              for(k = kk; k<((kk+q)>n?n:(kk+q)); k++){
                                        temp += A[i][k]*B[k][j];
                              }
                              C[i][j] += temp;
                      }
              }
      }
  }
  */
  MPI_Gather (C[num], n*n/q, MPI_INT, C, n*n/q, MPI_INT, 0, MPI_COMM_WORLD);
//Print Matrix
  if (rank==0) {
      printf("  Matrix A \n");
      printf("--------------");
    for (i=0; i<n; i++) {
      printf("\n"); 
      for (j=0; j<n; j++){
        printf("%3d ", A[i][j]);
      }
    }
    printf("\n"); 
    printf("\n   Matrix B \n");
    printf("--------------");

     for (i=0; i<n; i++) {
      printf("\n"); 
      for (j=0; j<n; j++){
        printf("%3d ", B[i][j]);
      }
    }
    printf("\n"); 
    printf("\nRESULT: Matrix C \n");
    printf("----------------");

    for (i=0; i<n; i++) {
      printf("\n"); 
      for (j=0; j<n; j++){
        printf("%3d ", C[i][j]);
      }
    }
    printf("\n"); 
  }

  gettimeofday(&end, NULL); 
  time_taken = (end.tv_sec - start.tv_sec) * 1e6; 
  time_taken = (time_taken + (end.tv_usec - start.tv_usec)) * 1e-6;
  printf( "\nTotal time = %f seconds \n" , time_taken); 

  MPI_Finalize();
  return 0;
}




