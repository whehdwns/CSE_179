#include <stdio.h>
#include <mpi.h>
#define N 10

int main(int argc,  char **argv){
  int rank,  size;//, offset;
  int buf[N];
  int i;

  MPI_File fh;
  MPI_Status status;
  MPI_Offset offset, filesize;

  MPI_Init(&argc,  &argv);
  MPI_Comm_size(MPI_COMM_WORLD,  &size);
  MPI_Comm_rank(MPI_COMM_WORLD,  &rank);

//Task 1
// Open the file and write by using individual file pointers 
  for(i=0;i<N;i++) buf[i] =i;
  offset = rank * (N/size) *sizeof(int);
  MPI_File_open(MPI_COMM_WORLD,  "datafile",  MPI_MODE_CREATE|MPI_MODE_WRONLY,  MPI_INFO_NULL, &fh);
  printf("\nRank: %d, Offset: %lld\n", rank, offset);
  MPI_File_seek(fh, offset, MPI_SEEK_SET);
  MPI_File_write(fh, buf, (N/size), MPI_INT, &status); //individual file pointers
  MPI_File_close(&fh);

//Task 2
// Re-open the file and read by using explicit offset 
  MPI_File_open(MPI_COMM_WORLD, "datafile", MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
  MPI_File_get_size(fh, &filesize);
  offset =rank * (N/size) *sizeof(int);
  printf("rank %d, filesize %lld, offset %lld\n",  rank, filesize, offset);
  MPI_File_read_at(fh, offset, &buf, (N/size), MPI_INT, &status); //explicit offset 
  printf("rank %d,  after read:", rank);
  for(i=0;i<N; i++) printf("%d ", buf[i]);
  printf("\n\n");
  MPI_File_close(&fh);

  //Task 3
  // Each process writes the data just read in the following way 
  MPI_File_open(MPI_COMM_WORLD,  "datafile2",  MPI_MODE_CREATE|MPI_MODE_WRONLY,  MPI_INFO_NULL, &fh);
  offset =rank * (N/size) *sizeof(int);
  MPI_File_set_view(fh, offset, MPI_INT, MPI_INT, "native", MPI_INFO_NULL);
  MPI_File_write_all(fh, buf, (N/size), MPI_INT, &status);
  MPI_File_get_size(fh, &filesize);
  printf("rank %d,  second filesize %lld,  offset %lld\n",  rank, filesize, offset);
  MPI_File_close(&fh);

  MPI_Finalize();
  return 0;
}