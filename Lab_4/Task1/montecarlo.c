#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h> 
#define NUM_THREADS 32

  void *compute_pi( void * );
  int sample_points;
  int total_hits;
  int total_misses;
  int hits[ NUM_THREADS ];
  int sample_points_per_thread;
  //int num_thread;
  
void *compute_pi( void *s )   
{
    int seed, i, *hit_pointer;
    int local_hits;
    double x_coord, y_coord;
    //unsigned rand_r(unsigned *ctx);
    hit_pointer = (int *) s;
    seed = *hit_pointer;
    local_hits = 0;

    for( i=0; i < sample_points_per_thread; i++ )
    {
      x_coord = (double) (rand_r( &seed ))/(double)RAND_MAX -0.5;
      y_coord = (double) (rand_r( &seed ))/(double)RAND_MAX-0.5;
      if((x_coord*x_coord + y_coord*y_coord)< 0.25)
        local_hits++;
     }

    *hit_pointer = local_hits;
     pthread_exit(0); 
 }


  int main( int argc, char *argv[] )
  {
    int i;
    int retval;
    pthread_t p_threads[NUM_THREADS];
    pthread_attr_t attr;
    double computed_pi;
    struct timeval start, end;
    gettimeofday(&start, NULL);
    //  computed_pi();
    //gettimeofday(&end, NULL); 
    double time_taken;
    retval = 0;


    pthread_attr_init( &attr );
    pthread_attr_setscope( &attr, PTHREAD_SCOPE_SYSTEM );

    printf( "Enter number of sample points: " );
    scanf( "%d", &sample_points );
    //printf( "Enter number of threads: " );
    //scanf( "%d", &num_thread );

    total_hits = 0;
    sample_points_per_thread = sample_points / NUM_THREADS;

    for( i=0; i<NUM_THREADS; i++ )
    {
      hits[i] = i;
      pthread_create( &p_threads[ i ], &attr, compute_pi, (void *) &hits[i] );
    }

    for( i=0; i<NUM_THREADS; i++ )
    {
      pthread_join( p_threads[ i ], NULL );
      total_hits += hits[ i ];
      }

     computed_pi = 4.0 * (double) total_hits / ((double) (sample_points));
     gettimeofday(&end, NULL); 
    time_taken = (end.tv_sec - start.tv_sec) * 1e6; 
    time_taken = (time_taken + (end.tv_usec - 
                start.tv_usec)) * 1e-6; 


     printf( "Computed PI = %lf\n", computed_pi );
     printf( "Total time = %f seconds \n" , time_taken);

     return( retval );
   }