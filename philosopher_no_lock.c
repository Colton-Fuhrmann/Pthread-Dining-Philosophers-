/*************************************************************************//**
 * @file philosopher_no_lock.c
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
// This version will run forever. A philosopher can only grab the chopsticks
// to his/her left and right if both of them are available (i.e an adjacent
// philosopher isn't currently eating as well.
//
// ctrl-c is caught in this program and the eat and wait statistics of each
// philosopher is printed out. Speedup (Tserial/Tparallel) and
// Efficiency (Tserial/Tparallel*p) are also calculated and printed.
 *
 * @details 
 *
 * @section compile_section Compiling and Usage
 *
 * @par Compiling Instructions:
 *     
 *    gcc -g -Wall -o philosopher_no_lock philosopher_no_lock.c -lpthread
 *
 * @par Usage:
   @verbatim 
   c:\> philosopher_no_lock
   @endverbatim
 *
 *
 *****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

long thread_count;
long n;
const int NUM_PHILOSOPHERS = 5;
long int eat_count[5] = {0,0,0,0,0};
long int wait_count[5] = {0,0,0,0,0};
int chopsticks[5] = {1,1,1,1,1};
int waiting[5] = {0,0,0,0,0};

void *philosopher( void* thread );
void stop_philosophers();

pthread_mutex_t lock;

int main ( int argc, char **argv )
{
  long thread;
  pthread_t* thread_handles;

  signal(SIGINT, stop_philosophers);
  
  // set thread count to number of philosophers ( 5 )
  thread_count = NUM_PHILOSOPHERS;
  // initialize space for all threads
  thread_handles = malloc( thread_count * sizeof( pthread_t ));

  pthread_mutex_init(&lock, NULL);

  // create the pthreads on function void *philosopher(void* thread)
  for ( thread = 0; thread < thread_count; thread++ )
    pthread_create( &thread_handles[thread], NULL, philosopher, (void*) thread );

      // join all the thread handles
  for ( thread = 0; thread < thread_count; thread++ )
    pthread_join( thread_handles[thread], NULL );

  pthread_mutex_destroy(&lock);
  free( thread_handles );

  return 0;

}


void *philosopher( void* thread )
{
  long philosopher_rank = (long) thread;
  int rank = (int)philosopher_rank;

  while( 1 )
  {
    unsigned int seed = (unsigned int)(time(NULL)) + (long)(thread);
    if( waiting[rank] < 50 )
    {
    printf( "Philosopher %ld is thinking.\n", philosopher_rank );
    usleep((rand_r(&seed)%100)+ 100);
    }
    // philosopher stops thinking & becomes hungry
    
    // lock mutex shared by all philos, then check to see if both chopsticks
    // are available and if so, grab them
    pthread_mutex_lock(&lock);      
    if(( chopsticks[philosopher_rank] == 1 && chopsticks[(philosopher_rank + 1)%
	 NUM_PHILOSOPHERS] == 1 ))
    {
    // mark the chopsticks as taken
    chopsticks[philosopher_rank] = 0;
    chopsticks[(philosopher_rank + 1) % NUM_PHILOSOPHERS] = 0;

    // unlock shared mutex to allow others to check for available sticks
    pthread_mutex_unlock(&lock); 

    // when have both chopsticks, eat for awhile
    printf( "Philosopher %ld is eating.\n", philosopher_rank );
    usleep((rand_r(&seed)%100)+ 100);
    eat_count[philosopher_rank]++;  //increment eat count of philosopher

    pthread_mutex_lock(&lock);
    //after eating, put both chopsticks down by unlocking the mutexes
    printf( "Philosopher %ld puts down left and right choptick\n", philosopher_rank );

    //set chopsticks as grabbable and then unlock mutex
    chopsticks[philosopher_rank] = 1;
    chopsticks[(philosopher_rank + 1) % NUM_PHILOSOPHERS] = 1;
    pthread_mutex_unlock(&lock);

    waiting[rank] = 0;
    // start think/eat process over
  }    

  else
  {
    // increment the wait count and unlock mutex -> allow others to check sticks
    wait_count[philosopher_rank]++;
    pthread_mutex_unlock(&lock);
    
    // increment # of times spent waiting since last time philos ate
    waiting[philosopher_rank]++; 
  }

}

return NULL;

}

void stop_philosophers( )
{
  usleep(50000);
    //print the eat counts of all philosophers
   printf("Eat count:  0 = %10ld, 1 = %10ld, 2 = %10ld, 3 = %10ld, 4 = %10ld\n", 
   eat_count[0], eat_count[1], eat_count[2], eat_count[3], eat_count[4]);

  printf("Wait count:  0 = %10ld, 1 = %10ld, 2 = %10ld, 3 = %10ld, 4 = %10ld\n", 
   wait_count[0], wait_count[1], wait_count[2], wait_count[3], wait_count[4]);

  int i;
  for( i = 0; i < NUM_PHILOSOPHERS; i++ )
  {
  printf("Philosopher %d ate %5.2lf percent of the time\n", i, 100 * 
    ((double)(eat_count[i])/(eat_count[i] + wait_count[i])));
  }

  exit(1);

}
