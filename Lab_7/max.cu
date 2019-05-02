#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <ctime>
#define N 10000


__global__ void findmaximum(float *A, float *max,int n)
{
  int index = threadIdx.x + blockIdx.x*blockDim.x;
  int dim = gridDim.x*blockDim.x;
  int offset =0;
  float temp;
  while(index + offset < n){
    temp = fmaxf(temp, A[index + offset]);
    offset += dim;
  }
  __syncthreads();

}


int main()
{
  float *h_A;
  float *d_array;
  float *h_max;
  float *d_max;

  // allocate memory
  h_A = (float*)malloc(N*sizeof(float));
  h_max = (float*)malloc(sizeof(float));
  cudaMalloc((void**)&d_array, N*sizeof(float));
  cudaMalloc((void**)&d_max, sizeof(float));
  cudaMemset(d_max, 0, sizeof(float));

  for(int i=0;i<N;i++){
    h_A[i] = N*float(rand()) / RAND_MAX;
  }
  //GPU
  float gpu_elapsed_time;
  cudaEvent_t gpu_start;
  cudaEvent_t gpu_stop;
  cudaEventCreate(&gpu_start);
  cudaEventCreate(&gpu_stop);
  //start Time
  cudaEventRecord(gpu_start, 0);
  cudaMemcpy(d_array, h_A, N*sizeof(float), cudaMemcpyHostToDevice);

  for(int j=0;j<1000;j++){
    dim3 gridSize = 128;
    dim3 blockSize = 128;
    findmaximum<<< gridSize, blockSize >>>(d_array, d_max, N);
  }

  cudaMemcpy(h_max, d_max, sizeof(float), cudaMemcpyDeviceToHost);
  cudaEventRecord(gpu_stop, 0);
  cudaEventSynchronize(gpu_stop);
  cudaEventElapsedTime(&gpu_elapsed_time, gpu_start, gpu_stop);
  cudaEventDestroy(gpu_start);
  cudaEventDestroy(gpu_stop);
  //stop Time
  std::cout<<"Maximum number found on gpu was: "<<*h_max<<std::endl;
  std::cout<<"The gpu took: "<<gpu_elapsed_time<<" milli-seconds"<<std::endl;
  //CPU
  //start time
  clock_t cpu_start = clock();
  for(unsigned int j=0;j<1000;j++){
    *h_max = -1.0;
    for(unsigned int i=0;i<N;i++){
      if(h_A[i] > *h_max){
        *h_max = h_A[i];
      }
    }
  }
  //stop time
  clock_t cpu_stop = clock();
  clock_t cpu_elapsed_time = 1000*(cpu_stop - cpu_start)/CLOCKS_PER_SEC;

  std::cout<<"Maximum number found on cpu was: "<<*h_max<<std::endl;
  std::cout<<"The cpu took: "<<cpu_elapsed_time<<" milli-seconds"<<std::endl;

  free(h_A);
  free(h_max);
  cudaFree(d_array);
  cudaFree(d_max);
}


