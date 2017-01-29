libstackusage
=============
libstackusage is a small library which allows a user to determine the
stack usage of a Linux application (main thread and native pthread child
threads). A primary use case may be to determine stack utilization for child
threads, and adjust the stack size allocation to provide desired margin. This
is particularly useful for applications and libraries designed to work in
resource-constrained environments, such as embedded Linux systems.

Supported Platforms
===================
libstackusage is primarily developed and tested on Linux, but basic
functionality should work in OS X as well. Current version has been tested on:
- OS X El Capitan 10.11
- Ubuntu 16.04 LTS

Installation
============
Download the source code:

    git clone https://github.com/d99kris/libstackusage && cd libstackusage

Configure and build:

    ./configure && make

Optionally install in system:

    sudo make install

Usage
=====

Linux
-----

General usage syntax:

    [SU_STDERR=1] [SU_SYSLOG=1] LD_PRELOAD="/path/to/libstackusage.so" <application>

Example using libstackusage (without system install) with gedit and logging
output to stderr:

    SU_STDERR=1 LD_PRELOAD="./src/.libs/libstackusage.so" gedit

Example using libstackusage (system installed) with ls and logging
output to syslog:

    SU_SYSLOG=1 LD_PRELOAD="/usr/lib/libstackusage.so" ls

Example using libstackusage (without system install) with sutest01 application
logging output to stderr and syslog:

    SU_STDERR=1 SU_SYSLOG=1 LD_PRELOAD="./src/.libs/libstackusage.so" ./test/sutest01

OS X
----

General usage syntax:

    [SU_STDERR=1] [SU_SYSLOG=1] DYLD_INSERT_LIBRARIES="/path/to/libstackusage.1.dylib" DYLD_FORCE_FLAT_NAMESPACE=1 <application>

Example using libstackusage (without system install) with sutest01 application
logging output to stderr and syslog:

    SU_STDERR=1 SU_SYSLOG=1 DYLD_INSERT_LIBRARIES="./src/.libs/libstackusage.1.dylib" DYLD_FORCE_FLAT_NAMESPACE=1 ./test/sutest01

Output
======
Example output:

    $ SU_STDERR=1 LD_PRELOAD="./src/.libs/libstackusage.so" ./test/sutest01
    libstackusage log start -------------------------------------------------
      pid  id    tid  requested     actual     maxuse  max%    dur  funcP
    23930   0  23930    8388608    8384512       4144     0      0  (nil)
    23930   1  23931      16384      16384       9336    56      0  0x4009b0
    23930   2  23932      20480      20480      13528    66      0  0x4009b0
    23930   3  23933      24576      24576      16728    68      0  0x4009b0
    23930   4  23934      32768      32768      24920    76      0  0x4009b0
    23930   5  23935      49152      49152      41304    84      0  0x4009b0
    libstackusage log end ---------------------------------------------------

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

    $ addr2line -f -e ./test/sutest01 0x4009b0
    thread_start
    ./test/sutest01.c:101

Design Notes
============
libstackusage intercepts calls to pthread_create and fills the thread
stack with a dummy data pattern. It also registers a callback routine to be
called upon thread termination. For main thread stack it utilizes
constructor and destructor routines.

Limitations
===========
Logging to stderr upon process termination may not work for all applications.
Certain applications, like coreutils ls, etc, are closing stdout/stderr before
library destructor is called. For such applications it is recommended to
use syslog logging.

License
=======
libstackusage is distributed under BSD 3-Clause license. See LICENSE file.

Keywords
========
linux, os x, pthreads, stack usage, stack utilization, threads.

