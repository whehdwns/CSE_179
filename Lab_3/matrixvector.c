/*#include <stdio.h>
#include <omp.h>
#define N 1000
	float A[N][N];
	float B[N][N];
	float C[N][N];
//#define CHUNKSIZE 100

int main(){
	int i, j, k;
		#pragma omp parallel for
			for(i =0; i<N; i++){
				for( j= 0; j<N; j++){
					C[i][j] =0;
					for(k =0; k<N; k++){
						C[i][j] +=A[i][k] *B[k][j];
					}
				}
			}
			
			for(i= 0; i<N; i++){
				for(j =0 ; j<N; j++){
				printf("C: %f \n", C[i][j]);
				}
			}
		return 0;
}
*/
#include <stdio.h>
#include <omp.h>

int main() {
	//used random number of A and x value
	float a[2][2] = {{1,2},{3,4}};
	float x[] = {4,6};
	float c[2];
	int i,j;

	// computes A*b
	#pragma omp parallel shared(a,x,c) private(i,j)
	{
		#pragma omp for schedule(guided)
		for (i=0; i<2; i++) {
			c[i]=0;
			for (j=0;j<2;j++) {
				c[i]+=a[i][j]*x[j];
			}
		}
		for (i=0; i<2; i++) {
			printf("c[%i]=%f \n",i,c[i]);
		}
	}
	return 0; 	
}
