#include<stdio.h>
#include <cuda.h>
#include <sys/time.h>
__global__ void compute(int* x,int* y,int n){
        int col=threadIdx.x+blockIdx.x*blockDim.x;
        int row=threadIdx.y+blockIdx.y*blockDim.y;
        int num=col+row*n;
        int neighbor=0;
        //cell in the middle has eight neighbors,
        //a cell in a corner has only three neighbors,
        //a cell on an edge has five neighbors. 
    if(col<n && row<n){     
        //corner 3
            //In order to move to corner,
            //it should move either diagonal or move left/right and move up/down
        //top left corner
        if(col==0 && row==0){
            neighbor+=x[num+1]; //move right
            neighbor+=x[num+n];  //move bottom
            neighbor+=x[num+n+1]; //bottom right
        }
        //bottom left corner
        else if(col==0 && row==n-1){
            neighbor+=x[num+1];   //move right
            neighbor+=x[num-n];   //move up
            neighbor+=x[num-n+1]; //top right
        }
        //bottom right
        else if(col==n-1 && row==n-1){  
                neighbor+=x[num-1];   //move left
                neighbor+=x[num-n];  //move up
                neighbor+=x[num-n-1]; //top left
        }
        //edge 5
        //In order to move to edge
        // it should just move to left/right/up/down (including corner)
        //top edge
        else if(row==0 && col>0 && col<n-1){    
                neighbor+=x[num-1]; //left
                neighbor+=x[num+1]; //right
                neighbor+=x[num+n-1]; //bottom left -- corner
                neighbor+=x[num+n];   //bottom
                neighbor+=x[num+n+1]; //bottom right -- corner
        }
        //bottom edge
        else if(row==n-1 && col>0 && col<n-1){  
                neighbor+=x[num-1]; //left
                neighbor+=x[num+1]; //right
                neighbor+=x[num-n+1]; //Top right-- corner
                neighbor+=x[num-n];   //Top
                neighbor+=x[num-n-1]; //Top left -- corner   
        }
        //Left edge
        else if(col==0 && row>0 && row<n-1){    
                neighbor+=x[num+1]; //right
                neighbor+=x[num-n]; //Top
                neighbor+=x[num-n+1]; //Top right-- corner
                neighbor+=x[num+n];   //Bottom
                neighbor+=x[num+n+1]; //Bottom right-- corner     
        }
        //Right edge
        else if(col==n-1 && row>0 && row<n-1){  
                neighbor+=x[num-1]; //left
                neighbor+=x[num-n]; //Top
                neighbor+=x[num-n-1]; //Top left-- corner
                neighbor+=x[num+n];   //Bottom
                neighbor+=x[num+n-1]; //Bottom left-- corner       
        }
         //Right edge
        else if(col==n-1 && row>0 && row<n-1){  
                neighbor+=x[num-1]; //left
                neighbor+=x[num-n]; //Top
                neighbor+=x[num-n-1]; //Top left-- corner
                neighbor+=x[num+n];   //Bottom
                neighbor+=x[num+n-1]; //Bottom left-- corner       
        }

        //cell in the middle has eight neighbors,        
        else{   
                neighbor+=x[num-1];//left  
                neighbor+=x[num+1];   //right
                neighbor+=x[num-n-1];//top left
                neighbor+=x[num-n];    //top
                neighbor+=x[num-n+1];  //top right
                neighbor+=x[num+n-1];  //bottom left
                neighbor+=x[num+n];    //bottom
                neighbor+=x[num+n+1];  //bottom right
        }
        //Die : 0
        //Live: 1
        //A live cell with zero or one live neighbor dies from loneliness.
        if(x[num]==1 && (neighbor ==0 || neighbor ==1))
                y[num]=0;
        //A live cell with four or more live neighbors dies due to overpopulation.
        else if(x[num]==1 && neighbor>=4)
                y[num]=0;
        //A dead cell with two or three live neighbors becomes alive.
        else if(x[num]==1 && (neighbor==2 || neighbor==3))
                y[num]=1;
        //Otherwise, a cell's state stays unchanged
        else
            y[num] = x[num];
    }
}


int main(void){
        int i,j,k;
        int row= 6;
        int col= 4;
        int start[row][col];
        int Round[row][col];
        dim3 threadsPerBlock(32,32);
        dim3 numBlocks(row/threadsPerBlock.x,col/threadsPerBlock.x);
        int* x;
        int* y;
        int generation =1;// maximum generation/iteration
        float milliseconds=0;
        cudaEvent_t t_start,t_stop;
        cudaEventCreate(&t_start);
        cudaEventCreate(&t_stop);
        //Initilazie the matrix of the x Generated cell
        for(i=0;i<row;i++)
                for(j=0;j<col;j++)
                        start[i][j]=rand()%2;
         //Initilazie the matrix of the y Generated cell
        for(i=0;i<row;i++)
                for(j=0;j<col;j++)
                        Round[i][j]=0;
   
        cudaMalloc((void **) &x,sizeof(int)*row*col);
        cudaMemcpy(x,start,sizeof(int)*row*col,cudaMemcpyHostToDevice);


        printf("Start\n");
        printf("-------\n");
        for(i=0;i<row;i++)
        {
                for(j=0;j<col;j++)
                {
                    if(start[i][j]){
                        printf(" 0");
                    }
                    else{
                        printf(" 1");
                    }
                }
                printf("\n");
        }

        for(k=0;k<= generation;k++)
        {  
            cudaEventRecord(t_start);
            compute<<<numBlocks,threadsPerBlock>>>(y,y,row);
            cudaEventRecord(t_stop);
            cudaMalloc((void **) &y,sizeof(int)*row*col);

            cudaMemcpy(Round,y,sizeof(int)*row*col,cudaMemcpyDeviceToHost);

                printf("\n Round %d \n",k);
                printf("-------\n");
                    for(i=0;i<row;i++)
                    {
                        for(j=0;j<col;j++)
                        {
                            if(Round[i][j])
                            {
                                   printf(" 0");
                            }
                            else{
                                printf(" 1");
                            }
                        }
                        printf("\n");
                    }   
        cudaEventElapsedTime(&milliseconds,t_start,t_stop);
        printf("Time taken for this computation = %f milliseconds\n\n",milliseconds);
      
    }
        

        return 0;
}


