#include <pthread.h>
#include <stdlib.h>
#define NUM_THREADS 32

void *compute_pi (void *);

int main()
{

	pthread_t p_threads[NUM_THREADS];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	for(i=0; i<NUM_THREADS; i++){
		hits[i] = i;
		pthread_create(&p_threads[i], &attr, compute_pi, (void*) &hits[i]);
	}
	for(i =0; i<NUM_THREADS; i++){
		pthread_join(p_threads[i], NULL);
		total_hits +=hits[i];
	}
}
	
	
	void *compute_pi (void *s) 
	{ 
     int seed, i, *hit_pointer; 
      double rand_no_x, rand_no_y; 
      int local_hits; 
 
      hit_pointer = (int *) s; 
      seed = *hit_pointer; 
      local_hits = 0; 
      for (i = 0; i < sample_points_per_thread; i++) { 
          rand_no_x =(double)(rand_r(&seed))/(double)((2<<14)-1); 
          rand_no_y =(double)(rand_r(&seed))/(double)((2<<14)-1); 
          if (((rand_no_x - 0.5) * (rand_no_x - 0.5) + 
              (rand_no_y - 0.5) * (rand_no_y - 0.5)) < 0.25) 
              local_hits ++; 
          seed *= i; 
      } 
      *hit_pointer = local_hits; 
      pthread_exit(0); 
  } 

