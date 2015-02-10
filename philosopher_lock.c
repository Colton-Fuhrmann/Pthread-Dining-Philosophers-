/*************************************************************************//**
 * @file philosopher_lock.c
 *
 * @mainpage Assignment 1
 *
 * @section course_section Course Information
 *
 * @author Colton Fuhrmann
 *
 * @date October 17, 2014
 *
 * @par Professor:
 *         Karlsson, Christer
 *
 * @par Course:
 *         CSC410 - M001 -  MWF 3:00PM
 *
 * @par Location:
 *         McLaury - 313
 *
 * @section program_section Program Information
 *
// 5 philosophers around round table with a chopstick in between each adjacent
// philospher.
// Each philosopher can do two things: think and eat
// A philosopher will eat for a while and then stops and becomes hungry. When
// a philosopher becomes hungry, he/she cannot eat until he/she owns the
// chopsticks to his/her left and right. When done eating, the chopsticks are
// put down and he/she begins thinking again.
//
// This version will deadlock (i.e. every philosopher holds a chopstick). This
// code will show and/or describe the scenarios where the philosophers end in
// a deadlock.
// The way a deadlock happens in this particular flavor of the dining 
// philosophers problem is whenever a philosopher is hungry, they grab the
// chopstick to their left. When every philosopher grabs the chopstick to
// their left, there is no right chopstick for any to grab, and thus the
// deadlock occurs.
 *
 * @details 
 *
 * @section compile_section Compiling and Usage
 *
 * @par Compiling Instructions:
 *     
 *    gcc -g -Wall -o philosopher_lock philosopher_lock.c -lpthread
 *
 * @par Usage:
   @verbatim 
   c:\> philosopher_lock
   @endverbatim
 *
 *
 *****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

long thread_count;		  // number of threads that will run
const int NUM_PHILOSOPHERS = 5;	  // const used to symbolize philosophers
pthread_mutex_t chop_mutex[5];   // 1 chopstick symbolizes a mutex 

void *philosopher( void* thread );

int main ( int argc, char **argv )
{
  long i;
  pthread_t* thread_handles;

  // set thread count to number of philosophers ( 5 )
  thread_count = NUM_PHILOSOPHERS;
  // initialize space for NUM_PHILOSOPHER threads
  thread_handles = malloc( thread_count * sizeof( pthread_t ));

  for ( i = 0; i < NUM_PHILOSOPHERS; i++ )
    pthread_mutex_init( &chop_mutex[i], NULL );

  // create the pthreads on function void *philosopher(void* thread)
  for ( i = 0; i < thread_count; i++ )
    pthread_create( &thread_handles[i], NULL, philosopher, (void*) i );

  // join all the thread handles
  for ( i = 0; i < thread_count; i++ )
    pthread_join( thread_handles[i], NULL );
 
  // destory mutex array and free memory associated with thread_handles
  for ( i = 0; i < NUM_PHILOSOPHERS; i++ )
    pthread_mutex_destroy( &chop_mutex[i] ); 
  free( thread_handles );

  return 0;
}

// This is the function where the philosopher goes through the think/eat cycle.
// Each thread will loop through here acting as a philosopher. usleep is used
// to mimic the philosopher thinking & eating.
void *philosopher( void* thread )
{
  long philosopher_rank = (long) thread;

  while( 1 )
  {
    // create a seed to use in rand_r for each loop iteration
    unsigned int seed = (unsigned int)(time(NULL)) + (long)(thread);

    // philosopher thinks for a while
    usleep((rand_r(&seed)%100) + 10);
    printf( "Philosopher %ld is thinking.\n", philosopher_rank );   

    // philosopher stops thinking & becomes hungry
    // philosopher checks for left chopstick & picks up if available
    pthread_mutex_lock(&chop_mutex[philosopher_rank]);
    printf( "Philosopher %ld picks up left chopstick.\n", philosopher_rank );
	  
    // philosopher picks up right chopstick & picks up if available
    pthread_mutex_lock(&chop_mutex[(philosopher_rank + 1) % NUM_PHILOSOPHERS]);
    printf( "Philosopher %ld picks up right chopstick.\n", philosopher_rank );

    // when have both chopsticks, eat for awhile
    printf( "Philosopher %ld is eating.\n", philosopher_rank );
    usleep((rand_r(&seed)%100)+ 10);

    // after eating, need to unlock both chopsticks so neighbors may eat
    printf( "Philosopher %ld puts down left and right chopstick\n",
	    philosopher_rank );
    pthread_mutex_unlock(&chop_mutex[philosopher_rank]);
    pthread_mutex_unlock(&chop_mutex[(philosopher_rank + 1) % NUM_PHILOSOPHERS]);

    // start think/eat process over
  }  

  return NULL;
}
