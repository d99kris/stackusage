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
libstackusage should work on most Linux systems with glibc present. It has
however only been tested on:
Ubuntu 14.04 LTS / Intel x64

Installation
============
Download the source code package:

    wget https://github.com/d99kris/libstackusage/archive/master.zip

Extract package:

    unzip master.zip

Configure and build:

    ./configure && make

Optionally install in system:

    sudo make install

Usage
=====
General usage syntax:

    [SU_STDERR=1] [SU_SYSLOG=1] LD_PRELOAD="/path/to/libstackusage.so" <application>

Example using libstackusage (without system install) with gedit and logging
output at process stop to stderr:

    SU_STDERR=1 LD_PRELOAD="./src/.libs/libstackusage.so" gedit

Example using libstackusage (system installed) with ls and logging
output at process stop to syslog:

    SU_SYSLOG=1 LD_PRELOAD="/usr/lib/libstackusage.so" ls

Example using libstackusage (without system install) with suctest application
logging output at process stop to stderr and syslog:

    SU_STDERR=1 SU_SYSLOG=1 LD_PRELOAD="./src/.libs/libstackusage.so" ./test/suctest
 
Output
======
Example output:

    $ SU_STDERR=1 LD_PRELOAD="./src/.libs/libstackusage.so" ./test/suctest
    libstackusage   pid tid  requested     actual    maxuse  max%    dur  funcP
    libstackusage  3830   0    8388608    8384512     34584     0      0  (nil)
    libstackusage  3830   1      16384      16384     11496    70      0  0x4009d0
    libstackusage  3830   2      16384      16384     13560    82      0  0x4009d0
    libstackusage  3830   3      32768      32768     28552    87      0  0x4009d0

Description of columns:

    pid         process id
    tid         thread id (0 = main thread, 1 = first child thread, etc..)
    requested   requested stack size in bytes
    actual      actual available stack size in bytes (excl. any stack guard)
    maxuse      maximum stack usage in bytes
    max%        maximum use of requested stack size in percentage
    dur         thread duration in seconds
    funcP       (nil) for main, pointer to start_routine for child threads

The function that funcP points to can be determined for example using addr2line:

    $ addr2line -f -e ./test/suctest 0x4009d0
    thread_start
    ./test/suctest.c:76

Design Notes
============
libstackusage intercepts calls to pthread_create and fills the thread
stack with a dummy data pattern. It also registers a callback routine to be
called upon thread termination. For main thread stack it utilizes
__constructor__ and __destructor__ routines.

Limitations
===========
Logging to stderr upon process termination may not work for all applications.
Certain applications, like coreutils ls, etc, are closing stdout/stderr before
library __destructor__ is called. For such applications it is recommended to
use syslog logging.

License
=======
libstackusage is distributed under BSD 3-Clause license. See LICENSE file.

Keywords
========
pthreads, linux, threads, stack usage, stack utilization.

