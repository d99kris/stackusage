/*
 * suctest.c
 *
 * Copyright (c) 2015, Kristofer Berggren
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the author nor the names of its contributors may
 *       be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
#define STACK_PER_CALL  1000


/* ----------- Local Function Prototypes ------------------------- */
static pthread_t spawn_thread(int stack_size, int *stack_usage,
                              pthread_attr_t *attr);
static void *thread_start(void *arg);


/* ----------- File Global Variables ----------------------------- */
static int stack_usage_main = 30000;
static int stack_usage_t1 = 1000;
static int stack_usage_t2 = 5000;
static int stack_usage_t3 = 20000;
static int stack_usage_t4 = 7000;


/* ----------- Global Functions ---------------------------------- */
int main(int argc, char *argv[], char *envp[])
{
  pthread_t t1, t2, t3, t4;
  pthread_attr_t attr1, attr2, attr3, attr4;

  /* Spawn threads with some arbitrary stack sizes and usages */
  t1 = spawn_thread(PTHREAD_STACK_MIN, &stack_usage_t1, &attr1);
  usleep(10000);
  t2 = spawn_thread(PTHREAD_STACK_MIN, &stack_usage_t2, &attr2);
  usleep(10000);
  t3 = spawn_thread(PTHREAD_STACK_MIN*2, &stack_usage_t3, &attr3);
  usleep(10000);
  t4 = spawn_thread(PTHREAD_STACK_MIN, &stack_usage_t4, &attr4);
  usleep(10000);

  /* Use stack in main-thread */
  thread_start(&stack_usage_main);

  /* Wait for threads to complete */
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  pthread_join(t3, NULL);
  pthread_join(t4, NULL);

  return 0;
}


/* ----------- Local Functions ----------------------------------- */
static pthread_t spawn_thread(int stack_size, int *stack_usage,
                              pthread_attr_t *attr)
{
  pthread_t tid;

  pthread_attr_init(attr);
  pthread_attr_setstacksize(attr, stack_size);
  pthread_create(&tid, attr, &thread_start, stack_usage);
  pthread_attr_destroy(attr);

  return tid;
}


static void *thread_start(void *arg)
{
  int *stack_usage = (int *)arg;
  {
    /* Using GCC extension allowing dynamic stack allocation */
    char dummy_data[*stack_usage];

    /* Populate stack with dummy data */
    int i = 0;
    for(i = 0; i < *stack_usage; i++)
    {
      dummy_data[i] = rand() & 0xff;
      if((dummy_data[i] > 0x7f) && (i > 0))
      {
        dummy_data[i] = dummy_data[i - 1];
      }
    }
  }

  /* Exercise both explicit and implicit thread termination */
  if(rand() % 2)
  {
    pthread_exit(NULL);
  }

  return NULL;
}

