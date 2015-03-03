/*
 * sucpptest.cc
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

#include <iostream>
#include <thread>
#include <unistd.h>

static void thread_start(int recursion_depth);

int main()
{
  // Spawn threads
  std::thread thread1(thread_start, 0);
  usleep(10000);
  std::thread thread2(thread_start, 10);
  usleep(10000);
  std::thread thread3(thread_start, 100);
  usleep(10000);
  std::thread thread4(thread_start, 1000);
  usleep(10000);
  std::thread thread5(thread_start, 10000);
  usleep(10000);
  std::thread thread6(thread_start, 100000);

  // Wait for child threads to terminate
  thread1.join();
  thread2.join();
  thread3.join();
  thread4.join();
  thread5.join();
  thread6.join();

  return 0;
}

static void thread_start(int recursion_depth)
{
  char dummy = 'A';
  if(recursion_depth > 0)
  {
    dummy++;
    thread_start(recursion_depth - 1);
  }
  else
  {
    dummy++;
    std::cout << "Completed recursion (" << dummy << ")" << std::endl;
  }
  return;
}

