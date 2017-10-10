Stackusage
==========

| **Linux + Mac** |
|-----------------|
| [![Build status](https://travis-ci.org/d99kris/stackusage.svg?branch=master)](https://travis-ci.org/d99kris/stackusage) |

Stackusage measures stack usage in Linux and macOS applications (main thread and
native pthread child threads). The measured stack utilization data can be used to
adjust the stack size allocation to provide desired margin for child threads. This
is primarily useful for applications and libraries designed to work in
resource-constrained environments (such as embedded systems).

Example Usage
=============

    $ stackusage ./ex001
    stackusage log start ----------------------------------------------------
      pid  id    tid  requested     actual     maxuse  max%    dur  funcP
    23930   0  23930    8388608    8384512       4144     0      0  (nil)
    23930   1  23931      16384      16384       9336    56      0  0x4009b0
    23930   2  23932      20480      20480      13528    66      0  0x4009b0
    23930   3  23933      24576      24576      16728    68      0  0x4009b0
    23930   4  23934      32768      32768      24920    76      0  0x4009b0
    23930   5  23935      49152      49152      41304    84      0  0x4009b0
    stackusage log end ------------------------------------------------------

Supported Platforms
===================
Stackusage is primarily developed and tested on Linux, but basic
functionality should work in macOS / OS X as well. Current version has been tested on:
- OS X El Capitan 10.11
- Ubuntu 16.04 LTS

Limitation: On macOS / OS X this tool relies on code injection using DYLD_INSERT_LIBRARIES,
which generally does not work with third-party applications in a standard system. Using it on
(your own) applications built from source should work fine though.

Installation
============
Pre-requisites (Ubuntu):

    sudo apt install git cmake build-essential

Download the source code:

    git clone https://github.com/d99kris/stackusage && cd stackusage

Generate Makefile and build:

    mkdir -p build && cd build && cmake .. && make -s

Optionally install in system:

    sudo make install

Usage
=====

General usage syntax:

    stackusage <application> [application arguments]

Example checking stack usage of test program 'ex001' with stackusage installed on system:

    stackusage ./ex001

Output Format
=============
Example output:

    stackusage log start ----------------------------------------------------
      pid  id    tid  requested     actual     maxuse  max%    dur  funcP
    23930   0  23930    8388608    8384512       4144     0      0  (nil)
    23930   1  23931      16384      16384       9336    56      0  0x4009b0
    23930   2  23932      20480      20480      13528    66      0  0x4009b0
    23930   3  23933      24576      24576      16728    68      0  0x4009b0
    23930   4  23934      32768      32768      24920    76      0  0x4009b0
    23930   5  23935      49152      49152      41304    84      0  0x4009b0
    stackusage log end ------------------------------------------------------

Description of columns:

    pid         process id
    id          id (0 = main thread, 1 = first child thread, etc..)
    tid         thread id / tid (only supported on Linux, others show '0')
    requested   requested stack size in bytes
    actual      actual stack size in bytes (excl. any stack guard)
    maxuse      maximum stack usage in bytes
    max%        maximum use of requested stack size in percentage
    dur         thread duration in seconds
    funcP       (nil) for main, pointer to start_routine for child threads

The function that funcP points to can be determined for example using addr2line:

    $ addr2line -f -e ./ex001 0x4009b0
    thread_start
    ./tests/ex001.c:81

Technical Details
=================
Stackusage intercepts calls to pthread_create and fills the thread
stack with a dummy data pattern. It also registers a callback routine to be
called upon thread termination. In the callback routine the amount of remaining
dummy pattern in the stack is checked, in order to determine the stack usage.

License
=======
Stackusage is distributed under the BSD 3-Clause license. See LICENSE file.

Keywords
========
linux, macos, os x, pthreads, stack usage, stack utilization, threads.

