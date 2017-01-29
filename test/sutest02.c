/*
 * sutest02.c
 *
 * Copyright (c) 2017, Kristofer Berggren
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
#define NUM_THREADS  (PTHREAD_KEYS_MAX + 1)


/* ----------- Local Function Prototypes ------------------------- */
static pthread_t spawn_thread(void);
static void *thread_start(void *arg);


/* ----------- Global Functions ---------------------------------- */
int main(int argc, char *argv[], char *envp[])
{
  pthread_t tid[NUM_THREADS];
  int i = 0;

  /* Spawn threads */
  for(i = 0; i < NUM_THREADS; i++)
  {
    tid[i] = spawn_thread();
  }

  /* Wait for threads to complete */
  for(i = 0; i < NUM_THREADS; i++)
  {
    pthread_join(tid[i], NULL);
  }
  
  return 0;
}


/* ----------- Local Functions ----------------------------------- */
static pthread_t spawn_thread(void)
{
  pthread_t tid;
  pthread_attr_t attr;

  pthread_attr_init(&attr);
  pthread_attr_setstacksize(&attr, PTHREAD_STACK_MIN);
  pthread_create(&tid, &attr, &thread_start, NULL);
  pthread_attr_destroy(&attr);

  return tid;
}


static void *thread_start(void *arg)
{
  /* Sleep 1 ms */
  usleep(1000);

  return NULL;
}

