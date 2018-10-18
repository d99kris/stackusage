/*
 * ex003.c
 *
 * Copyright (C) 2015-2018 Kristofer Berggren
 * All rights reserved.
 * 
 * stackusage is distributed under the BSD 3-Clause license, see LICENSE for details.
 *
 */

/* ----------- Includes ------------------------------------------ */
#include <assert.h>
#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


/* ----------- Local Function Prototypes ------------------------- */
static void *thread_start(void *arg);
static void preinit_func(void);


/* ----------- Local Variables ----------------------------------- */
/* GCC supports a preinit_array to be called before regular     
 * __attribute__ ((constructor)) functions. In this test such a 
 * preinit function is registered, which in turns spawns a child
 * thread. This tests stackusage capability to intercept 
 * pthread_create calls before stackusage general initialization
 * has been performed.
 */
__attribute__((section(".preinit_array"))) static void (*funcs[])(void) =
{
  &preinit_func
};


/* ----------- Global Functions ---------------------------------- */
int main(void)
{
  assert(funcs != NULL);
  return 0;
}


/* ----------- Local Functions ----------------------------------- */
static void *thread_start(void *arg)
{
  sleep(1);
  return arg;
}

static void preinit_func(void)
{
  pthread_t thread;
  assert(pthread_create(&thread, NULL, &thread_start, NULL) == 0);
}
