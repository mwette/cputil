# cputil - a valgrind tool for estimating CPU utilization

Copyright (C) 2016-2018 -- Matthew R. Wette.

Permission is granted to copy, distribute and/or modify this document
under the terms of the GNU Free Documentation License, Version 1.3 or
any later version published by the Free Software Foundation; with no
Invariant Sections, no Front-Cover Texts, and no Back-Cover Texts.  A
copy of the license is included with the distribution as COPYING.DOC.

The cputil program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

## Introduction

Note: The repository has branches, vg3.11 and vg3.13, cooresponding to 
ports for each of the valgrind releases.  That is,
* If you are working with valgrind 3.11.0, check out the vg3.11 branch.
* If you are working with valgrind 3.13.0, check out the vg3.13 branch.

This valgrind tool provides approximate cycle counts for user-defined
regions of code.  It works on a per-thread basis.  The following program,
when executed under cputil/valgrind will report the estimated number of 
clock cycles required to execute foo().  The default processor model is
a PowerPC750, but can be changed with a user-loadable table.

```
#include <stdio.h>
#include "cputil.h"
int main() {
  long cycles;
  CU_REGTHR();                /* register the current thread */
  for (i = 0; i < 5; i++) {
    CU_CLRCTR();              /* clear the cycle counter */
    foo(i);
    cycles += CU_GETCTR();    /* read the cycle counter */
    bar(i);
  }
  printf("clk-cycles=%d\n", cycles)
}
```

Valgrind works by first converting binary programs to execute on
the valgrind virtual CPU called VEX.  In the process of converting
binary (e.g., x86_64 machine code) to VEX, the cputil tool add
instructions to keep clock counts for executed code.  The user
can add calls (CLRCTR and GETCTR) to clear and read the counter.
The number of clock counts associated with each instruction are
provided in a table.  The default table can be replaced with a 
user generated table at runtime.

## Installation

This works with valgrind-3.11.0 and valgrind-3.13.0.  
Our goal is to be compatible with major releases.  

To build and install for valigrind-3.13.0:

1. Checkout the 3.13 branch of cputil:
   ```
   $ git clone -b vg3.13.0 https://github.com/mwette/cputil
   ```

2. Download and unpack valgrind-3.13.0

3. Clone or otherwise install the cputil distribution into a (new)
   directory called *cputil* under valgrind-3.11.0.  It should look like
```
AUTHORS				 darwin15.supp
COPYING				 darwin16.supp
...
coregrind/			 tests/
cputil/				 valgrind.pc.in
darwin10-drd.supp		 valgrind.spec
...
darwin12.supp			 xfree-3.supp
   
```

4. In the subdirectory cputil, execute *patch1*:
   ```
   $ pwd
   .../valgring-3.13.0/cputil
   $ ./patch1
   ```

5. In the valgrind top directory run configure, make and install:
   ```
   $ ./configure --prefix=/usr/local
   $ make 
   $ make install
   ```

5. Optionally, if you want a minimal distribution, do this:
   ```
   $ ./configure --prefix=/usr/local
   $ mkdir -p /var/tmp/bindist
   $ make DESTDIR=/var/tmp/bindist pkglibdir=/usr/local/lib/cputil
   $ make DESTDIR=/var/tmp/bindist pkglibdir=/usr/local/lib/cputil install
   $ sh cputil/mindist /var/tmp/bindist /usr/local/lib/cputil
   ```

### Dumping the op-count tables
To dump the internal tables execute
   ```
   $ valgrind --tool=cputil --dump-op-tables=dump.cut true
   ```
There is no way to dump the op-tables w/o specifing a program to so
we use use /bin/true.


### Loading user-defined op-count tables
A dumped op table can be edited for clock counts and used for another run:
   ```
   $ cp dump.cut load.cut
   $ edit load.cut
   $ valgrind --tool=cputil --load-op-tables=load.cut my_program
   ```

### Threads

Support for multi-threaded code has been added.  Each thread must be 
registered with cputil.

### Demo

Look in cputil/tests/demo1.c
