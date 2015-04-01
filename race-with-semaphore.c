// 
//      race.c - demonstrate a Race Condition
//
//      To compile:
//              gcc -lpthread -o race race_with_semaphore.c
//
//      To run:
//              race
//

          
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <time.h>
//#include <iostream>

pthread_t tid1, tid2;

// Function prototypes
void *pthread1(void *), *arg1;
void *pthread2(void *), *arg2;
sem_t mutex;

// This is the global variable shared by both threads, initialised to 50
// Both threads will try to update its value simultaneously
int     theValue = 50;

// Now the main function. A function is called a method in Java
int main(int argc, char *argv[])
{
        int     err;
	sem_init(&mutex, 0, 1);

	// initialise the random number generator to sleep for random time
        srand (getpid());

        arg1 = ((void*)atoi(argv[1]));
        arg2 = ((void*)atoi(argv[2]));

        // try to start pthread 1 by calling pthread_create()
        err = pthread_create(&tid1, NULL, pthread1, arg1);
        if(err)
         {
          printf ("\nError in creating the thread 1: ERROR code %d \n", err);
          return 1;
         }

        // try to start pthread 2 by calling pthread_create()
        err = pthread_create(&tid2, NULL, pthread2, arg2);
        if (err)
         {
          printf ("\nError in creating the thread 1: ERROR code %d \n", err);
          return 1;
         }

        // wait for both threads to complete
        pthread_join(tid1, NULL);
        pthread_join(tid2, NULL);

	// display the final value of variable theValue
       printf ("The final value of theValue is %d \n", theValue);
}


// this is the first thread - it increments the global variable theValue
//consider this thread as the producer of the sensore values

void *pthread1(void *param)
{
  time_t startTime;
  time(&startTime);

  int a = *((int *)param);
  printf("\nthread 1 started\n");
  for (int i = 0; i <= 5; ++i)
  {
      sem_wait(&mutex);
            

    //***** The critical section of thread 1
            sleep(rand() & 1);      // encourage race condition
            theValue += (a + i);
            /*
            a = theValue;
            sleep(rand() & 1);      // encourage race condition
            a += (a + i);                    // increment the value of theValue by 1
            sleep(rand() & 1);      // encourage race condition
            theValue = a;
            */
    //***** The end of the critical section of thread 1

            
      sem_post(&mutex);
  }

  time_t endTime;
  time(&endTime);

  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("\nthread 1 now terminating after %s \n", ctime(&curtime));
}


// this is the second thread - it decrements the global variable theValue
void *pthread2(void *param)
{
  time_t startTime;
  time(&startTime);


  int b = *((int *)param);
  //  (b-5) + (b-4) + (b-3) + (b-2) + (b-1)

  printf("\nthread 2 started\n");
  for (int i = 5; i > 0; --i)
  {
    //Check the buffer is free  - has released by the sensor thread
      sem_wait(&mutex);

    //***** The critical section of thread 2

      theValue += (b - i);
            /*
            sleep(rand() & 1);      // encourage race condition
            a = theValue;
            sleep(rand() & 1);      // encourage race condition
            a += (a - i);                    // decrement the value of theValue by 1
            sleep(rand() & 1);      // encourage race condition
            theValue = a;
            */
    //***** The end of the critical section of thread 2
      sem_post(&mutex);
  }

  time_t endTime;
  time(&endTime);

  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

  printf("\nthread 2 now terminating %s \n", ctime(&curtime));
}
