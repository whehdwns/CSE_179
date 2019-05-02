#include <stdio.h>
#include <mpi.h>
#define SIZE 16
#define UP    0
#define DOWN  1
#define LEFT  2
#define RIGHT 3
int main(int argc, char *argv[])
{
  int numtasks, rank, source, dest, outbuf, i, tag=1, 
      inbuf[4]={MPI_PROC_NULL,MPI_PROC_NULL,MPI_PROC_NULL,MPI_PROC_NULL,}, 
      nbrs[4], dims[2]={4,4}, 
      periods[2]={0,0}, reorder=0, coords[2];
  float avg;
  int temp;

  MPI_Comm cartcomm;
  MPI_Status stats[8];
  MPI_Request reqs[8];

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (numtasks == SIZE){
    MPI_Dims_create(numtasks, 2, dims);
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 1, &cartcomm);
    //task 1
    MPI_Comm_rank(cartcomm, &rank);
    //MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Cart_coords(cartcomm, rank, 2, coords);

    MPI_Cart_shift(cartcomm, 0, 1, &nbrs[UP], &nbrs[DOWN]);
    MPI_Cart_shift(cartcomm, 1, 1, &nbrs[LEFT], &nbrs[RIGHT]);

     printf("rank= %d coords= %d %d  neighbors(u,d,l,r)= %d %d %d %d\n",
             rank,coords[0],coords[1],nbrs[UP],nbrs[DOWN],nbrs[LEFT],
             nbrs[RIGHT]);

    //Task 2
    /* south */
    outbuf = rank;
    MPI_Cart_shift(cartcomm, 1, 1, &source, &dest);
    MPI_Sendrecv(&rank, 1, MPI_INT, dest, 0, &temp, 1, MPI_INT,source, 0, cartcomm, stats);
    outbuf += temp;

    /* north */
    MPI_Cart_shift(cartcomm, 1, -1, &source, &dest);
    MPI_Sendrecv(&rank, 1, MPI_INT, dest, 0, &temp, 1, MPI_INT,source, 0, cartcomm, stats);
    outbuf += temp;

    /*east */
    MPI_Cart_shift(cartcomm, 0, 1, &source, &dest);
    MPI_Sendrecv(&rank, 1, MPI_INT, dest, 0, &temp, 1, MPI_INT,source, 0, cartcomm, stats);
    outbuf += temp;

    /* west */
    MPI_Cart_shift(cartcomm, 0, -1, &source, &dest);
    MPI_Sendrecv(&rank, 1, MPI_INT, dest, 0, &temp, 1, MPI_INT, source, 0, cartcomm, stats);
    outbuf += temp;
    // average between local rank and south,north, east, and west neighbors
    avg = (float) outbuf / 5;
    printf("rank %d (%d, %d), neighbours average: %.2f\n", rank, coords[0], coords[1], avg);


    for (i=0; i<4; i++) {
         dest = nbrs[i];
         source = nbrs[i];
         MPI_Isend(&outbuf, 1, MPI_INT, dest, tag, 
                   MPI_COMM_WORLD, &reqs[i]);
         MPI_Irecv(&inbuf[i], 1, MPI_INT, source, tag, 
                   MPI_COMM_WORLD, &reqs[i+4]);
         }

      MPI_Waitall(8, reqs, stats);
   
      printf("rank= %d                  inbuf(u,d,l,r)= %d %d %d %d\n",
             rank,inbuf[UP],inbuf[DOWN],inbuf[LEFT],inbuf[RIGHT]); 


  }else{
      printf("Must specify %d processors. Terminating.\n",SIZE);
  }
  MPI_Finalize();

  return 0;
}
