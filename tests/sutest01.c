/*
 * sutest01.c
 *
 * Copyright (C) 2015-2017 Kristofer Berggren
 * All rights reserved.
 * 
 * stackusage is distributed under the BSD 3-Clause license, see LICENSE for details.
 *
 */

/* ----------- Includes ------------------------------------------ */
#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


/* ----------- Defines ------------------------------------------- */
#define NUM_THREADS  5


/* ----------- Local Function Prototypes ------------------------- */
static pthread_t spawn_thread(int *stack_usage);
static void *thread_start(void *arg);


/* ----------- File Global Variables ----------------------------- */
static int thread_stack_usage[NUM_THREADS] =
{
  0,
  4096,
  8192,
  16384,
  32768,
};


/* ----------- Global Functions ---------------------------------- */
int main(void)
{
  pthread_t tid[NUM_THREADS];
  int i = 0;

  /* Spawn threads */
  for(i = 0; i < NUM_THREADS; i++)
  {
    tid[i] = spawn_thread(&thread_stack_usage[i]);
    usleep(10000);
  }

  /* Wait for threads to complete */
  for(i = 0; i < NUM_THREADS; i++)
  {
    pthread_join(tid[i], NULL);
  }
  
  return 0;
}


/* ----------- Local Functions ----------------------------------- */
static pthread_t spawn_thread(int *stack_usage)
{
  pthread_t tid;
  pthread_attr_t attr;

  pthread_attr_init(&attr);
  pthread_attr_setstacksize(&attr, PTHREAD_STACK_MIN + *stack_usage);
  printf("Starting thread size=%d usage=%d\n",
         PTHREAD_STACK_MIN + *stack_usage, *stack_usage);
  pthread_create(&tid, &attr, &thread_start, stack_usage);
  pthread_attr_destroy(&attr);

  return tid;
}


static void *thread_start(void *arg)
{
  int *stack_usage = (int *)arg;
  if(*stack_usage > 0)
  {
    /* Allocate memory on the stack */
    char dummy_data[*stack_usage];

    /* Populate with dummy data */
    for(int i = 0; i < *stack_usage; i++)
    {
      dummy_data[i] = (dummy_data[i] + rand()) & 0xff;
    }
  }
  return NULL;
}

